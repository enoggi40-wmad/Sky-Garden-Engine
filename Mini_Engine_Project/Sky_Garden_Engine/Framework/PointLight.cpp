#include "PointLight.h"
#include "GameObject.h"
#include "Transform.h"

#include <algorithm>

using namespace DirectX;

PointLight::PointLight(GameObject* owner)
    : Light(owner)
{
}

XMFLOAT3 PointLight::GetPosition() const
{
    return gameObject->transform.GetWorldPosition();
}

void PointLight::SetRange(float range)
{
    m_Range = std::max(0.0f, range);
}

float PointLight::GetRange() const
{
    return m_Range;
}

void PointLight::SetAttenuation(const XMFLOAT3& attenuation)
{
    m_Attenuation = attenuation;
}

XMFLOAT3 PointLight::GetAttenuation() const
{
    return m_Attenuation;
}

