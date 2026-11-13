#pragma once

#include "Component.h"

class Transform;

class Orbiter : public Component
{
public:
    Orbiter(
        GameObject* ownerObject,
        Transform* center,
        float radius,
        float speed,
        float startAngle
    );

    void Update(float deltaTime) override;

private:
    Transform* m_Center;
    float m_Radius;
    float m_Speed;
    float m_Angle = 0.0f;
};