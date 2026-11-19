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

	void SetColor(const DirectX::XMFLOAT4& color) 
	{ 
		m_Color = color; 
	}

	DirectX::XMFLOAT4 GetColor() const 
	{ 
		return m_Color; 
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
	DirectX::XMFLOAT4 m_Color = { 1.0f, 1.0f, 1.0f, 1.0f };

	float m_Intensity = 1.0f;
};