
#include "Orbiter.h"
#include "GameObject.h"

#include <cmath>

using namespace DirectX;

Orbiter::Orbiter(
    GameObject* ownerObject,
    XMFLOAT3 center,
    float radius,
    float orbitSpeed,
    float startAngle)
    : Component(ownerObject),
    m_Center(center),
    m_Radius(radius),
    m_OrbitSpeed(orbitSpeed),
    m_Angle(startAngle)
{
}


void Orbiter::Update(float deltaTime)
{
    // 기본 공전 중심
    XMFLOAT3 center = m_Center;

    GameObject* parent = gameObject->GetParent();

    if (parent)
    {
        // 부모의 현재 월드 위치 가져오기
        XMFLOAT3 parentPosition =
            parent->transform.GetWorldPosition();

        // 부모 위치에 공전 중심 오프셋 추가
        center.x += parentPosition.x;
        center.y += parentPosition.y;
        center.z += parentPosition.z;
    }

    
}

