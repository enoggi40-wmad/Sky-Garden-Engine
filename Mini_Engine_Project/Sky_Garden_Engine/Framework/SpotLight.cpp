#include "SpotLight.h"
#include "GameObject.h"
#include "Transform.h"

#include <algorithm>

using namespace DirectX;

SpotLight::SpotLight(GameObject* owner)
    : Light(owner)
{
}

XMFLOAT3 SpotLight::GetPosition() const
{
    return gameObject->transform.GetWorldPosition();
}

XMFLOAT3 SpotLight::GetDirection() const
{
    return gameObject->transform.Forward();
}

void SpotLight::SetRange(float range)
{
    m_Range = std::max(0.0f, range);
}

float SpotLight::GetRange() const
{
    return m_Range;
}

void SpotLight::SetAttenuation(const XMFLOAT3& attenuation)
{
    m_Attenuation = attenuation;
}

XMFLOAT3 SpotLight::GetAttenuation() const
{
    return m_Attenuation;
}

void SpotLight::SetInnerAngle(float angle)
{
    m_InnerAngle = std::clamp(angle, 0.0f, m_OuterAngle);
}

float SpotLight::GetInnerAngle() const
{
    return m_InnerAngle;
}

void SpotLight::SetOuterAngle(float angle)
{
    m_OuterAngle = std::clamp(angle, m_InnerAngle, 179.0f);
}

float SpotLight::GetOuterAngle() const
{
    return m_OuterAngle;
}
