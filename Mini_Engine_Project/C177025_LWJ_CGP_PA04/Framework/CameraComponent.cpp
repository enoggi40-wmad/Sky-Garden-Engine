#include "CameraComponent.h"
#include "GameObject.h"

using namespace DirectX;

CameraComponent::CameraComponent(GameObject* owner)
    : Component(owner)
{
    m_ViewMatrix = XMMatrixIdentity();
    m_IsActive = false;
}

CameraComponent::~CameraComponent()
{
}

void CameraComponent::Update(float deltaTime)
{
}

void CameraComponent::SetActive(bool active)
{
    m_IsActive = active;
}

bool CameraComponent::IsActive() const
{
    return m_IsActive;
}

void CameraComponent::Render()
{
    XMVECTOR up, position, lookAt;
    float yaw, pitch, roll;
    XMMATRIX rotationMatrix;

    // 카메라의 위쪽 방향 벡터 설정
    // 기본적으로 Y축 위쪽 방향을 사용한다.
    up = XMVectorSet(
        0.0f,
        1.0f,
        0.0f,
        0.0f
    );


    // CameraComponent가 붙어있는 GameObject의
    // Transform 위치를 가져온다.
    XMFLOAT3 cameraPosition =
        gameObject->transform.position;

    // XMFLOAT3 형태의 위치 값을
    // DirectX 연산에 사용할 XMVECTOR 형태로 변환한다.
    position =
        XMLoadFloat3(&cameraPosition);


    // 카메라가 기본적으로 바라보는 방향을 설정한다.
    // DirectX 왼손 좌표계에서 +Z 방향을 기본 Forward로 사용한다.
    lookAt = XMVectorSet(
        0.0f,
        0.0f,
        1.0f,
        0.0f
    );


    // CameraComponent가 붙어있는 GameObject의
    // Transform 회전 값을 가져온다.
    XMFLOAT3 cameraRotation =
        gameObject->transform.rotation;


    // Transform의 회전 값은 Degree 단위이므로
    // DirectX 회전 함수에서 사용할 수 있도록 Radian으로 변환한다.
    pitch =
        cameraRotation.x * 0.0174532925f;

    yaw =
        cameraRotation.y * 0.0174532925f;

    roll =
        cameraRotation.z * 0.0174532925f;


    // Pitch, Yaw, Roll 값을 이용해
    // 카메라의 회전 행렬을 생성한다.
    rotationMatrix =
        XMMatrixRotationRollPitchYaw(
            pitch,
            yaw,
            roll
        );


    // 기본 Forward 방향에 회전을 적용해
    // 실제 카메라가 바라보는 방향을 계산한다.
    lookAt =
        XMVector3TransformCoord(
            lookAt,
            rotationMatrix
        );


    // Up 방향에도 동일한 회전을 적용한다.
    // 이를 통해 카메라의 Roll 회전도 View Matrix에 반영된다.
    up =
        XMVector3TransformCoord(
            up,
            rotationMatrix
        );


    // lookAt은 현재까지 방향 벡터이므로,
    // 카메라 위치를 더해 실제 바라보는 지점을 계산한다.
    lookAt =
        position + lookAt;


    // 카메라 위치, 바라보는 지점, 위쪽 방향을 이용해
    // 최종 View Matrix를 생성한다.
    m_ViewMatrix =
        XMMatrixLookAtLH(
            position,
            lookAt,
            up
        );
}

void CameraComponent::GetViewMatrix(
    XMMATRIX& viewMatrix)
{
    viewMatrix = m_ViewMatrix;
}