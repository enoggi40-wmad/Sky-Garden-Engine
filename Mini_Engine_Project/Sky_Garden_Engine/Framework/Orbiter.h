
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
    // 부모 원점 기준 공전 중심 오프셋 (월드 단위)
    DirectX::XMFLOAT3 m_Center;

    // 월드 단위 공전 반지름
    float m_Radius;

    // 공전 속도 (라디안/초)
    float m_OrbitSpeed;

    // 현재 공전 각도
    float m_Angle;
};
