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

DirectX::XMFLOAT3 CameraComponent::GetPosition() const
{
    return gameObject->transform.GetWorldPosition();
}

void CameraComponent::Render()
{
    // Transform에서 View Matrix 가져오기
    m_ViewMatrix =
        gameObject->transform.GetViewMatrix();
}

void CameraComponent::GetViewMatrix(
    XMMATRIX& viewMatrix)
{
    viewMatrix = m_ViewMatrix;
}