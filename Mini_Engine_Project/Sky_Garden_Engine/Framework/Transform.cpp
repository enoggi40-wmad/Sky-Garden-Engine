
#include "Transform.h"
#include <cmath>

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

// 자신의 로컬 위치, 회전, 크기로 부모 기준 변환 행렬 생성
XMMATRIX Transform::GetLocalMatrix() const
{
    // 로컬 크기 행렬 생성
    XMMATRIX scaleMatrix =
        XMMatrixScaling(
            scale.x,
            scale.y,
            scale.z
        );

    // 로컬 회전 행렬 생성
    XMMATRIX rotationMatrix =
        XMMatrixRotationRollPitchYaw(
            rotation.x,
            rotation.y,
            rotation.z
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

    // 행렬에서 크기, 회전, 위치 분리
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
    XMFLOAT3 newRotation;

    XMStoreFloat3(&newScale, scaleVector);
    XMStoreFloat3(&newPosition, positionVector);

    // 분해된 회전 쿼터니언을 회전 행렬로 변환
    XMFLOAT4X4 rotationMatrix;

    XMStoreFloat4x4(
        &rotationMatrix,
        XMMatrixRotationQuaternion(rotationQuaternion)
    );

    // 회전 행렬에서 X축 회전값 계산
    float sinPitch = -rotationMatrix._32;

    // asin 입력값이 [-1, 1] 범위를 벗어나지 않도록 제한
    if (sinPitch > 1.0f)
    {
        sinPitch = 1.0f;
    }
    else if (sinPitch < -1.0f)
    {
        sinPitch = -1.0f;
    }

    newRotation.x = std::asin(sinPitch);

    // 짐벌락이 발생하지 않는 일반적인 경우
    if (std::fabs(std::cos(newRotation.x)) > 0.0001f)
    {
        newRotation.y = std::atan2(
            rotationMatrix._31,
            rotationMatrix._33
        );

        newRotation.z = std::atan2(
            rotationMatrix._12,
            rotationMatrix._22
        );
    }
    else
    {
        // X축 회전이 +-90도 부근인 경우
        newRotation.y = std::atan2(
            -rotationMatrix._13,
            rotationMatrix._11
        );

        newRotation.z = 0.0f;
    }

    // 분해한 위치, 회전, 크기로 행렬을 다시 생성
    XMMATRIX rebuiltMatrix =
        XMMatrixScaling(
            newScale.x,
            newScale.y,
            newScale.z
        ) *
        XMMatrixRotationRollPitchYaw(
            newRotation.x,
            newRotation.y,
            newRotation.z
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

    // 원래 행렬과 복원한 행렬 비교
    for (int row = 0; row < 4; row++)
    {
        for (int column = 0; column < 4; column++)
        {
            float value = original.m[row][column];
            float restored = rebuilt.m[row][column];

            // 유효하지 않은 값이 있으면 실패
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

    // 모든 검증이 성공한 경우에만 Transform 변경
    m_LocalPosition = newPosition;
    rotation = newRotation;
    scale = newScale;

    return true;
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

// 부모 기준 로컬 위치 설정
void Transform::SetLocalPosition(const XMFLOAT3& localPosition)
{
    m_LocalPosition = localPosition;
}

// 부모 기준 로컬 위치 반환
XMFLOAT3 Transform::GetLocalPosition() const
{
    return m_LocalPosition;
}


