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
    float m_Amplitude;    // 위아래 이동 거리
    float m_Speed;        // 움직임 속도
    float m_Time;         // 누적 시간
    float m_StartPhase;   // 시작 위상값
    float m_StartY;       // 처음 Y 위치
};