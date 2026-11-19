#include "DirectionalLight.h"
#include "GameObject.h"
#include "Transform.h"

using namespace DirectX;

DirectionalLight::DirectionalLight(GameObject* owner) :Light(owner)
{

}

XMFLOAT3 DirectionalLight::GetDirection() const
{
	return gameObject->transform.Forward();
}