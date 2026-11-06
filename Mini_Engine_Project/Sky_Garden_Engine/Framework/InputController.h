#pragma once

#include "Component.h"

class GameObject;

class InputController : public Component
{
public:
	InputController(
		GameObject* ownerObject, 
		float moveSpeed = 300.0f
	);
	
	virtual void Update(float deltaTime) override;
private:
	float m_MoveSpeed;
};