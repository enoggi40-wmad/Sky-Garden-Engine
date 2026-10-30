#pragma once

#include "Scene.h"

class ResourceManager;
class CameraComponent;

class TestScene : public Scene
{
public:
    TestScene();
    ~TestScene();

    void Initialize(ResourceManager* resourceManager) override;
    void Update(float deltaTime) override;

private:
    CameraComponent* m_MainCamera = nullptr;
};