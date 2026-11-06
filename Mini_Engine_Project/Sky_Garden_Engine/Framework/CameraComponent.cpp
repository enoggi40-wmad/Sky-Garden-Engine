
#include "CameraComponent.h"
#include "GameObject.h"

CameraComponent::CameraComponent(GameObject* owner)
    : Component(owner)
{
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

const Transform& CameraComponent::GetTransform() const
{
    return gameObject->transform;
}
