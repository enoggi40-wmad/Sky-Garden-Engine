
#include "Orbiter.h"
#include "GameObject.h"
#include "Transform.h"

using namespace DirectX;

Orbiter::Orbiter(
    GameObject* ownerObject,
    Transform* center,
    float speed)
    : Component(ownerObject),
    m_Center(center),
    m_Speed(speed)
{
}

void Orbiter::Update(float deltaTime)
{
    if (!m_Center)
        return;

    gameObject->transform.RotateAround(
        m_Center->GetWorldPosition(),
        { 0.0f, 1.0f, 0.0f },
        m_Speed * deltaTime
    );
}
