#pragma once

#include "Component.h"
#include <DirectXMath.h>

class GameObject;

class CameraComponent : public Component
{
public:
	CameraComponent(GameObject* owner);
	~CameraComponent();

	void Update(float deltaTime) override;

	void Render();
	void GetViewMatrix(
		DirectX::XMMATRIX& viewMatrix
	);

private:
	DirectX::XMMATRIX m_ViewMatrix;

};