#include "Orbiter.h"
#include "GameObject.h"

#include <cmath>

Orbiter::Orbiter(
    GameObject* ownerObject,
    DirectX::XMFLOAT3 center,
    float radius,
    float orbitSpeed,
    float startAngle)
    : Component(ownerObject),
    m_Center(center),
    m_Radius(radius),
    m_OrbitSpeed(orbitSpeed),
    m_Angle(startAngle)
{
    // 컴포넌트를 붙인 순간의 로컬 높이를 저장한다.
    m_Height =
        gameObject->transform.GetLocalPosition().y;
}

void Orbiter::Update(float deltaTime)
{
    // 공전 각도 증가 ? 라디안 기준
    m_Angle += m_OrbitSpeed * deltaTime;

    // 부모 기준으로 원 위의 위치를 계산한다.
    DirectX::XMFLOAT3 localPosition =
    {
        m_Center.x + cosf(m_Angle) * m_Radius,
        m_Height,
        m_Center.z + sinf(m_Angle) * m_Radius
    };

    // 부모 기준 위치로 적용한다.
    gameObject->transform.SetLocalPosition(localPosition);
}