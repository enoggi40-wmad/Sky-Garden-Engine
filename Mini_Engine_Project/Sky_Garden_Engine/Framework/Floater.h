
#pragma once

#include "Component.h"

class Floater : public Component
{
public:
    Floater(
        GameObject* ownerObject,
        float amplitude,
        float speed,
        float startPhase = 0.0f
    );

    void Update(float deltaTime) override;

private:
    float m_Amplitude;
    float m_Speed;
    float m_Time;
    float m_StartPhase;
    float m_StartY;
};
