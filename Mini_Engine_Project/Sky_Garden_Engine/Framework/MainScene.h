#pragma once
#include "Scene.h"
#include <memory>
#include <vector>

class ResourceManager;
class CameraComponent;
class Material;

class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();

	void Initialize(ResourceManager* resourceManager) override;
	void Update(float deltaTime) override;

private:
	CameraComponent* m_MainCamera;
	CameraComponent* m_CutsceneCamera;


};