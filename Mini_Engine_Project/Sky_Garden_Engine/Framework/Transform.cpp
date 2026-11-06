
#include "Transform.h"
#include <cmath>

// 자신의 로컬 행렬에 부모의 월드 행렬을 곱해 최종 월드 행렬 계산
XMMATRIX Transform::GetWorldMatrix() const
{
    // 자신의 위치, 회전, 크기로 만든 로컬 행렬
    XMMATRIX localMatrix = GetLocalMatrix();

    // 부모가 있으면 부모의 월드 변환까지 반영
    if (m_Parent)
    {
        return localMatrix * m_Parent->GetWorldMatrix();
    }

    // 부모가 없으면 로컬 변환이 곧 월드 변환
    return localMatrix;
}

bool Transform::SetWorldMatrix(const XMMATRIX& worldMatrix)
{
    // 부모가 없으면 월드 행렬이 곧 로컬 행렬
    if (!m_Parent)
    {
        return TrySetLocalMatrix(worldMatrix);
    }

    // 부모의 월드 행렬 가져오기
    XMMATRIX parentWorldMatrix =
        m_Parent->GetWorldMatrix();

    // 부모 행렬의 행렬식 검사
    float determinant =
        XMVectorGetX(
            XMMatrixDeterminant(parentWorldMatrix)
        );

    // 역행렬을 구할 수 없으면 실패
    if (!std::isfinite(determinant) ||
        std::fabs(determinant) < 0.000001f)
    {
        return false;
    }

    // 부모의 월드 변환을 제거해서 로컬 행렬 계산
    XMMATRIX localMatrix =
        worldMatrix *
        XMMatrixInverse(nullptr, parentWorldMatrix);

    // 로컬 행렬을 위치, 회전, 크기로 분해해서 저장
    return TrySetLocalMatrix(localMatrix);
}


// 자신의 로컬 위치, 회전, 크기로 부모 기준 변환 행렬 생성
XMMATRIX Transform::GetLocalMatrix() const
{
    // 로컬 크기 행렬 생성
    XMMATRIX scaleMatrix =
        XMMatrixScaling(
            m_LocalScale.x,
            m_LocalScale.y,
            m_LocalScale.z
        );

    // 로컬 회전 행렬 생성
    // 주소 연산자 사용 - 포인터 변수를 따로 만들 필요 없음
    XMMATRIX rotationMatrix =
        XMMatrixRotationQuaternion(
            XMLoadFloat4(&m_LocalRotation)
        );

    // 로컬 위치 행렬 생성
    XMMATRIX translationMatrix =
        XMMatrixTranslation(
            m_LocalPosition.x,
            m_LocalPosition.y,
            m_LocalPosition.z
        );

    return scaleMatrix * rotationMatrix * translationMatrix;
}

// 전달받은 행렬을 위치, 회전, 크기로 분해해서 Transform에 저장
bool Transform::TrySetLocalMatrix(const XMMATRIX& matrix)
{
    XMVECTOR scaleVector;
    XMVECTOR rotationQuaternion;
    XMVECTOR positionVector;

    // 1. 행렬을 Scale, Rotation, Position으로 분해
    if (!XMMatrixDecompose(
        &scaleVector,
        &rotationQuaternion,
        &positionVector,
        matrix))
    {
        return false;
    }

    XMFLOAT3 newScale;
    XMFLOAT3 newPosition;
    XMFLOAT4 newRotation;

    // 2. Scale과 Position 저장
    XMStoreFloat3(&newScale, scaleVector);
    XMStoreFloat3(&newPosition, positionVector);

    // 3. Quaternion 정규화 후 저장
    XMStoreFloat4(
        &newRotation,
        XMQuaternionNormalize(rotationQuaternion)
    );

    // 4. 분해한 값으로 행렬 재생성
    XMMATRIX rebuiltMatrix =
        XMMatrixScaling(
            newScale.x,
            newScale.y,
            newScale.z
        ) *
        XMMatrixRotationQuaternion(
            XMLoadFloat4(&newRotation)
        ) *
        XMMatrixTranslation(
            newPosition.x,
            newPosition.y,
            newPosition.z
        );

    XMFLOAT4X4 original;
    XMFLOAT4X4 rebuilt;

    XMStoreFloat4x4(&original, matrix);
    XMStoreFloat4x4(&rebuilt, rebuiltMatrix);

    // 5. 원래 행렬과 복원한 행렬 비교
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            float value = original.m[row][column];
            float restored = rebuilt.m[row][column];

            // 유효하지 않은 값 검사
            if (!std::isfinite(value) ||
                !std::isfinite(restored))
            {
                return false;
            }

            // 부동소수점 오차 허용
            float tolerance =
                0.0001f * (1.0f + std::fabs(value));

            if (std::fabs(value - restored) > tolerance)
            {
                return false;
            }
        }
    }

    // 6. 검증 성공 시에만 Transform에 적용
    m_LocalPosition = newPosition;
    m_LocalRotation = newRotation;
    m_LocalScale = newScale;

    return true;
}

// 부모 변환까지 적용한 월드 위치 반환
XMFLOAT3 Transform::GetWorldPosition() const
{
    // 부모의 위치, 회전, 크기까지 적용한 월드 행렬
    XMMATRIX worldMatrix = GetWorldMatrix();

    // 자신의 원점을 월드 좌표로 변환
    XMVECTOR worldPosition =
        XMVector3TransformCoord(
            XMVectorZero(),
            worldMatrix
        );

    // 계산 결과를 XMFLOAT3로 저장해서 반환
    XMFLOAT3 result;
    XMStoreFloat3(&result, worldPosition);

    return result;
}


// 월드 좌표를 기준으로 위치 설정
bool Transform::SetWorldPosition(const XMFLOAT3& worldPosition)
{
    // 유효하지 않은 좌표는 저장하지 않는다.
    if (!std::isfinite(worldPosition.x) ||
        !std::isfinite(worldPosition.y) ||
        !std::isfinite(worldPosition.z))
    {
        return false;
    }

    // 부모가 없으면 월드 위치와 로컬 위치가 같다.
    if (!m_Parent)
    {
        m_LocalPosition = worldPosition;
        return true;
    }

    // 부모의 월드 행렬
    XMMATRIX parentWorldMatrix =
        m_Parent->GetWorldMatrix();

    // 부모의 행렬식 계산
    float determinant =
        XMVectorGetX(
            XMMatrixDeterminant(parentWorldMatrix)
        );

    // 역행렬을 만들 수 없는 경우 변경하지 않는다.
    if (!std::isfinite(determinant) ||
        std::fabs(determinant) < 0.000001f)
    {
        return false;
    }

    // 부모의 월드 행렬을 역행렬로 변환
    XMMATRIX inverseParentWorld =
        XMMatrixInverse(
            nullptr,
            parentWorldMatrix
        );

    // 월드 좌표를 부모 기준 로컬 좌표로 변환
    XMVECTOR localPosition =
        XMVector3TransformCoord(
            XMLoadFloat3(&worldPosition),
            inverseParentWorld
        );

    XMFLOAT3 result;
    XMStoreFloat3(&result, localPosition);

    // 변환 결과가 유효하지 않으면 기존 위치 유지
    if (!std::isfinite(result.x) ||
        !std::isfinite(result.y) ||
        !std::isfinite(result.z))
    {
        return false;
    }

    m_LocalPosition = result;

    return true;
}



// 부모 기준 로컬 위치 반환
XMFLOAT3 Transform::GetLocalPosition() const
{
    return m_LocalPosition;
}

// 부모 기준 로컬 위치 설정
void Transform::SetLocalPosition(const XMFLOAT3& localPosition)
{
    m_LocalPosition = localPosition;
}


// 부모 회전까지 적용한 월드 회전 반환
XMFLOAT4 Transform::GetWorldRotation() const
{
    // 자신의 로컬 Quaternion으로 회전 행렬 생성
    XMMATRIX localRotationMatrix =
        XMMatrixRotationQuaternion(
            XMLoadFloat4(&m_LocalRotation)
        );

    // 부모가 있으면 부모의 월드 회전 적용
    if (m_Parent)
    {
        XMFLOAT4 parentRotation =
            m_Parent->GetWorldRotation();

        XMMATRIX parentRotationMatrix =
            XMMatrixRotationQuaternion(
                XMLoadFloat4(&parentRotation)
            );

        localRotationMatrix =
            localRotationMatrix * parentRotationMatrix;
    }

    // 최종 회전 행렬을 Quaternion으로 변환
    XMVECTOR worldQuaternion =
        XMQuaternionNormalize(
            XMQuaternionRotationMatrix(localRotationMatrix)
        );

    XMFLOAT4 result;
    XMStoreFloat4(&result, worldQuaternion);

    return result;
}

// 월드 회전을 기준으로 회전 설정
void Transform::SetWorldRotation(const XMFLOAT4& rotation)
{
    // 부모가 없으면 월드 회전 = 로컬 회전
    if (!m_Parent)
    {
        SetLocalRotation(rotation);
        return;
    }

    // 원하는 월드 회전 행렬
    XMMATRIX desiredWorldRotation =
        XMMatrixRotationQuaternion(
            XMLoadFloat4(&rotation)
        );

    // 부모의 월드 회전 가져오기
    XMFLOAT4 parentRotation =
        m_Parent->GetWorldRotation();

    XMMATRIX parentRotationMatrix =
        XMMatrixRotationQuaternion(
            XMLoadFloat4(&parentRotation)
        );

    // 부모 회전을 제거하여 로컬 회전 계산
    // 회전 행렬의 역행렬은 전치행렬과 같음
    XMMATRIX localRotationMatrix =
        desiredWorldRotation *
        XMMatrixTranspose(parentRotationMatrix);

    // 로컬 회전 행렬을 Quaternion으로 변환
    XMVECTOR localQuaternion =
        XMQuaternionRotationMatrix(localRotationMatrix);

    XMFLOAT4 result;
    XMStoreFloat4(&result, localQuaternion);

    SetLocalRotation(result);
}


// 로컬 회전 반환 (Quaternion)
XMFLOAT4 Transform::GetLocalRotation() const
{
    return m_LocalRotation;
}

// 로컬 회전 설정 (Quaternion)
void Transform::SetLocalRotation(const XMFLOAT4& rotation)
{
    // Quaternion을 XMVECTOR로 변환
    XMVECTOR quaternion =
        XMLoadFloat4(&rotation);

    // Quaternion의 길이 제곱 계산
    float lengthSq =
        XMVectorGetX(XMVector4LengthSq(quaternion));

    // 유효하지 않은 Quaternion이면 변경하지 않음
    if (!std::isfinite(lengthSq) ||
        lengthSq < 0.000001f)
    {
        return;
    }

    // Quaternion 정규화 후 저장
    XMStoreFloat4(
        &m_LocalRotation,
        XMQuaternionNormalize(quaternion)
    );
}


// 월드 행렬에서 크기를 추출
bool Transform::GetWorldScale(XMFLOAT3& outScale) const
{
    XMMATRIX worldMatrix = GetWorldMatrix();

    XMVECTOR scaleVector;
    XMVECTOR rotationQuaternion;
    XMVECTOR positionVector;

    // 월드 행렬을 Scale, Rotation, Position으로 분해
    if (!XMMatrixDecompose(
        &scaleVector,
        &rotationQuaternion,
        &positionVector,
        worldMatrix))
    {
        return false;
    }

    // 분해한 값으로 행렬 복원
    XMMATRIX rebuiltMatrix =
        XMMatrixScalingFromVector(scaleVector) *
        XMMatrixRotationQuaternion(rotationQuaternion) *
        XMMatrixTranslationFromVector(positionVector);

    XMFLOAT4X4 original;
    XMFLOAT4X4 rebuilt;

    XMStoreFloat4x4(&original, worldMatrix);
    XMStoreFloat4x4(&rebuilt, rebuiltMatrix);

    // 복원된 행렬이 원본과 일치하는지 검증
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            float value = original.m[row][column];
            float restored = rebuilt.m[row][column];

            if (!std::isfinite(value) ||
                !std::isfinite(restored))
            {
                return false;
            }

            float tolerance =
                0.0001f * (1.0f + std::fabs(value));

            if (std::fabs(value - restored) > tolerance)
            {
                return false;
            }
        }
    }

    // 검증에 성공하면 월드 크기 반환
    XMStoreFloat3(&outScale, scaleVector);

    return true;
}


// 월드 크기를 기준으로 Scale 설정
bool Transform::SetWorldScale(const XMFLOAT3& worldScale)
{
    // 유효하지 않은 값 검사
    if (!std::isfinite(worldScale.x) ||
        !std::isfinite(worldScale.y) ||
        !std::isfinite(worldScale.z))
    {
        return false;
    }

    // 현재 월드 행렬 가져오기
    XMMATRIX worldMatrix = GetWorldMatrix();

    XMVECTOR currentScale;
    XMVECTOR currentRotation;
    XMVECTOR currentPosition;

    // 현재 월드 행렬 분해
    if (!XMMatrixDecompose(
        &currentScale,
        &currentRotation,
        &currentPosition,
        worldMatrix))
    {
        return false;
    }

    // 월드 행렬이 정확하게 분해 가능한지 확인
    XMFLOAT3 validatedScale;
    if (!GetWorldScale(validatedScale))
    {
        return false;
    }

    // 원하는 Scale과 기존 Rotation, Position으로
    // 새로운 월드 행렬 생성
    XMMATRIX newWorldMatrix =
        XMMatrixScaling(
            worldScale.x,
            worldScale.y,
            worldScale.z
        ) *
        XMMatrixRotationQuaternion(currentRotation) *
        XMMatrixTranslationFromVector(currentPosition);

    // 부모 변환을 역으로 제거해 로컬 값에 반영
    return SetWorldMatrix(newWorldMatrix);
}


// 로컬 크기 반환
XMFLOAT3 Transform::GetLocalScale() const
{
    return m_LocalScale;
}

// 로컬 크기 설정
void Transform::SetLocalScale(const XMFLOAT3& localScale)
{
    // 유효하지 않은 값은 저장하지 않음
    if (!std::isfinite(localScale.x) ||
        !std::isfinite(localScale.y) ||
        !std::isfinite(localScale.z))
    {
        return;
    }

    m_LocalScale = localScale;
}

void Transform::SetLocalEulerAngles(const XMFLOAT3& eulerDegrees)
{
    // Degree를 Radian으로 변환
    float pitch = XMConvertToRadians(eulerDegrees.x);
    float yaw = XMConvertToRadians(eulerDegrees.y);
    float roll = XMConvertToRadians(eulerDegrees.z);

    // Euler Angle을 Quaternion으로 변환
    XMVECTOR quaternion =
        XMQuaternionRotationRollPitchYaw(
            pitch,
            yaw,
            roll
        );

    // Quaternion 저장
    XMFLOAT4 result;
    XMStoreFloat4(&result, quaternion);

    SetLocalRotation(result);
}

XMFLOAT3 Transform::GetLocalEulerAngles() const
{
	
}


XMFLOAT3 Transform::Forward() const
{
    // 월드 회전 가져오기
    XMFLOAT4 rotation = GetWorldRotation();

    // 기본 전방(+Z)에 회전 적용
    XMVECTOR forward = XMVector3Rotate(
        XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
        XMLoadFloat4(&rotation)
    );

    XMFLOAT3 result;
    XMStoreFloat3(&result, forward);

    return result;
}

XMFLOAT3 Transform::Right() const
{
    // 월드 회전 가져오기
    XMFLOAT4 rotation = GetWorldRotation();

    // 기본 오른쪽(+X)에 회전 적용
    XMVECTOR right = XMVector3Rotate(
        XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f),
        XMLoadFloat4(&rotation)
    );

    XMFLOAT3 result;
    XMStoreFloat3(&result, right);

    return result;
}


void Transform::TranslateLocal(
    const XMFLOAT3& movement)
{
    // 현재 Local 위치 가져오기
    XMFLOAT3 position = GetLocalPosition();

    // 부모 기준 좌표 이동
    position.x += movement.x;
    position.y += movement.y;
    position.z += movement.z;

    SetLocalPosition(position);
}

bool Transform::TranslateWorld(
    const XMFLOAT3& movement)
{
    // 현재 World 위치 가져오기
    XMFLOAT3 position = GetWorldPosition();

    // 월드 기준 좌표 이동
    position.x += movement.x;
    position.y += movement.y;
    position.z += movement.z;

    // 부모 변환 역산은 기존 함수에 위임
    return SetWorldPosition(position);
}


void Transform::Rotate(const XMFLOAT3& eulerRadians)
{
    // 현재 로컬 회전 가져오기
    XMVECTOR current =
        XMLoadFloat4(&m_LocalRotation);

    // 추가할 회전을 Quaternion으로 생성
    XMVECTOR delta =
        XMQuaternionRotationRollPitchYaw(
            eulerRadians.x,
            eulerRadians.y,
            eulerRadians.z
        );

    // 현재 회전에 추가 회전 합성
    XMVECTOR result =
        XMQuaternionMultiply(delta, current);

    // 정규화하여 저장
    XMStoreFloat4(
        &m_LocalRotation,
        XMQuaternionNormalize(result)
    );
}



bool Transform::RotateAround(
    const XMFLOAT3& worldCenter,
    const XMFLOAT3& axis,
    float angleRadians)
{
    // 회전축 유효성 검사
    XMVECTOR rotationAxis = XMLoadFloat3(&axis);

    float lengthSq = XMVectorGetX(
        XMVector3LengthSq(rotationAxis)
    );

    if (!std::isfinite(lengthSq) ||
        lengthSq < 0.000001f ||
        !std::isfinite(angleRadians))
    {
        return false;
    }

    rotationAxis = XMVector3Normalize(rotationAxis);

    // 현재 월드 위치와 공전 중심
    XMFLOAT3 currentPosition = GetWorldPosition();

    XMVECTOR position = XMLoadFloat3(&currentPosition);
    XMVECTOR center = XMLoadFloat3(&worldCenter);

    // 중심에서 현재 위치까지의 벡터
    XMVECTOR offset = position - center;

    // 회전량 계산
    XMVECTOR rotation = XMQuaternionRotationAxis(
        rotationAxis,
        angleRadians
    );

    // 위치와 방향 회전
    XMVECTOR rotatedOffset =
        XMVector3Rotate(offset, rotation);

    XMFLOAT3 newPosition;
    XMStoreFloat3(
        &newPosition,
        center + rotatedOffset
    );

    // 월드 위치 설정
    return SetWorldPosition(newPosition);
}









