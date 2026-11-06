
#pragma once

#include "Component.h"

class GameObject;
class Transform;

class CameraComponent : public Component
{
public:
    CameraComponent(GameObject* owner);
    ~CameraComponent();

    void Update(float deltaTime) override;

    // 카메라 활성 상태
    void SetActive(bool active);
    bool IsActive() const;

    // 카메라가 속한 Transform 반환
    const Transform& GetTransform() const;

private:
    bool m_IsActive = false;
};
