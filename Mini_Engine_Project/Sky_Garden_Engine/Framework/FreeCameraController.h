#pragma once

#include "Component.h"
#include <Windows.h>

class GameObject;

class FreeCameraController : public Component
{
public:
    FreeCameraController(GameObject* owner);
    ~FreeCameraController();

    
    void Update(float deltaTime) override;

private:
    // 카메라 회전각 (Degree)
    float m_Pitch = 0.0f;
    float m_Yaw = 0.0f;
};