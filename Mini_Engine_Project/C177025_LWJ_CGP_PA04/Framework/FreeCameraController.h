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
};