#pragma once
#include "Scene.h"

class ResourceManager;

class MainScene : public Scene
{
public:
	MainScene();
	~MainScene();

	void Initialize(ResourceManager* resourceManager) override;
};