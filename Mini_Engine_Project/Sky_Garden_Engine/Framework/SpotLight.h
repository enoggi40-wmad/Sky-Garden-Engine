#pragma once

#include "Light.h"
#include <DirectXMath.h>

class SpotLight : public Light
{
public:
    explicit SpotLight(GameObject* owner);
    ~SpotLight() override = default;

    DirectX::XMFLOAT3 GetPosition() const;
    DirectX::XMFLOAT3 GetDirection() const;

    void SetRange(float range);
    float GetRange() const;

    void SetAttenuation(const DirectX::XMFLOAT3& attenuation);
    DirectX::XMFLOAT3 GetAttenuation() const;

    void SetInnerAngle(float angle);
    float GetInnerAngle() const;

    void SetOuterAngle(float angle);
    float GetOuterAngle() const;

private:
    float m_Range = 10.0f;

    // x = Constant, y = Linear, z = Quadratic
    DirectX::XMFLOAT3 m_Attenuation =
    {
        1.0f,
        0.09f,
        0.032f
    };

    // Degrees: full cone angles
    float m_InnerAngle = 20.0f;
    float m_OuterAngle = 40.0f;
};
