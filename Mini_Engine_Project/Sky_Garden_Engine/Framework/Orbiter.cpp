#include "Orbiter.h"
#include "GameObject.h"
#include "Transform.h"

using namespace DirectX;

Orbiter::Orbiter(
    GameObject* ownerObject,
    Transform* center,
    float radius,
    float speed,
    float startAngle)
    : Component(ownerObject),
    m_Center(center),
    m_Radius(radius),
    m_Speed(speed),
    m_Angle(startAngle)
{
}

void Orbiter::Update(float deltaTime)
{
    if (!m_Center)
        return;

    m_Angle += m_Speed * deltaTime;

    gameObject->transform.SetPositionAround(
        m_Center->GetWorldPosition(),
        { 0.0f, 1.0f, 0.0f },
        m_Radius,
        m_Angle
    );
}