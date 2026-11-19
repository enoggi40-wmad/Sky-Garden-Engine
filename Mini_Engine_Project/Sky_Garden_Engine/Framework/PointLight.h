#pragma once

#include "Light.h"
#include <DirectXMath.h>

class PointLight : public Light
{
	explicit PointLight(GameObject* owner);
	~PointLight() override = default;

	DirectX::XMFLOAT3 GetPosition() const;

	void SetRange(float range);
	float GetRange() const;

	// °Å¸® °¨¼è
	void SetAttenuation(const DirectX::XMFLOAT3& attenuation);
	DirectX::XMFLOAT3 GetAttenuation() const;

private:

	float m_Range = 10.0f;

	// x = Constant, y = Linear, z = Quadratic
	DirectX::XMFLOAT3 m_Attenuation =
	{
		1.0f,
		0.09f,
		0.032f
	};
};
