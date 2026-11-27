#pragma once
#include "Scene.h"

class ResourceManager;
class CameraComponent;

class SakuraScene : public Scene
{
public:
    SakuraScene();
    ~SakuraScene();

    void Initialize(ResourceManager* resourceManager) override;
    void Update(float deltaTime) override;

private:
    CameraComponent* m_MainCamera = nullptr;
    CameraComponent* m_CutsceneCamera = nullptr;
};