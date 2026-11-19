#pragma once

#include "Component.h"
#include <DirectXMath.h>

class GameObject;

class Light : public Component
{
public:
	Light(GameObject* owner)
		: Component(owner)
	{

	}

	virtual ~Light() = default;

	void SetDiffuseColor(const DirectX::XMFLOAT4& color) 
	{ 
		m_DiffuseColor = color; 
	}

	DirectX::XMFLOAT4 GetDiffuseColor() const 
	{ 
		return m_DiffuseColor; 
	}

	void SetIntensity(float intensity)
	{
		m_Intensity = intensity;
	}

	float GetIntensity() const 
	{
		return m_Intensity;
	}

protected:
	DirectX::XMFLOAT4 m_DiffuseColor = { 1.0f, 1.0f, 1.0f, 1.0f };

	float m_Intensity = 1.0f;
};