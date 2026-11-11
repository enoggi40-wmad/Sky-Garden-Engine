
#pragma once

#include "Component.h"

class GameObject;
class Transform;

class Orbiter : public Component
{
public:
    Orbiter(
        GameObject* ownerObject,
        Transform* center,
        float speed
    );

    void Update(float deltaTime) override;

private:
    Transform* m_Center = nullptr;
    float m_Speed = 0.0f;
};
