
#include "Orbiter.h"
#include "GameObject.h"
#include "Transform.h"

#include <cmath>

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
    Update(0.0f);
}

void Orbiter::Update(float deltaTime)
{
    if (!m_Center)
        return;

    m_Angle += m_Speed * deltaTime;

    // 공전 중심의 현재 월드 위치
    XMFLOAT3 center = m_Center->GetWorldPosition();

    // 월드 반지름만큼 떨어진 기준 위치
    if (!gameObject->transform.SetWorldPosition({
        center.x + m_Radius,
        center.y,
        center.z
        }))
    {
        return;
    }

    // 월드 중심을 기준으로 공전
    gameObject->transform.RotateAround(
        center,
        { 0.0f, 1.0f, 0.0f },
        m_Angle
    );
}
