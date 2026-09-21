// Rotator.h
#pragma once

#include "Component.h"

class Rotator : public Component
{
public:
    // 어떤 GameObject에 붙을지와 회전 속도를 받는다.
    Rotator(GameObject* ownerObject, float rotateSpeed)
        : Component(ownerObject), speed(rotateSpeed)
    {
    }

    // 매 프레임 호출
    void Update(float deltaTime) override;

private:
    // 회전 속도
    float speed;
};