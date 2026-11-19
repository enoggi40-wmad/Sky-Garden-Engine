#pragma once

#include "Light.h"
#include <DirectXMath.h>

class DirectionalLight : public Light
{
public:
	explicit DirectionalLight(GameObject* owner);
	~DirectionalLight() override = default;

	DirectX::XMFLOAT3 GetDirection() const;

};
