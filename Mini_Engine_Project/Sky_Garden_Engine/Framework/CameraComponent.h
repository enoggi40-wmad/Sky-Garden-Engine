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
	DirectX::XMFLOAT3 GetPosition() const;
private:
	DirectX::XMMATRIX m_ViewMatrix;

public:
	void SetActive(bool active);
	bool IsActive() const;

private:
	bool m_IsActive;



};