#pragma once

#include "Component.h"
#include <DirectXMath.h>

class Orbiter : public Component
{
public:
    Orbiter(
        GameObject* ownerObject,
        DirectX::XMFLOAT3 center,
        float radius,
        float orbitSpeed,
        float startAngle = 0.0f
    );

    void Update(float deltaTime) override;

private:
    // 공전 중심점
    DirectX::XMFLOAT3 m_Center;

    // 중심점으로부터의 거리
    float m_Radius;

    // 공전 속도
    float m_OrbitSpeed;

    // 현재 공전 각도
    float m_Angle;

    // 원래 Y 위치 유지용
    float m_Height;
};