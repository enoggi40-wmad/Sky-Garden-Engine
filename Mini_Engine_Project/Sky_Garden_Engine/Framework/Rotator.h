#pragma once

#include "Component.h"

class Rotator : public Component
{
public:
    Rotator(GameObject* ownerObject, float rotateSpeed)
        : Component(ownerObject),
        speed(rotateSpeed)
    {
    }

    void Update(float deltaTime) override;

    // 회전 속도 변경
    void SetSpeed(float newSpeed)
    {
        speed = newSpeed;
    }

    // 현재 회전 속도 확인
    float GetSpeed() const
    {
        return speed;
    }

private:
    float speed;
};