#pragma once

#include "ShaderType.h"
#include <DirectXMath.h>

class Material
{
public:
    Material();

    void SetShaderType(ShaderType type);
    ShaderType GetShaderType() const;

    void SetSpecularColor(const DirectX::XMFLOAT4& color);
    DirectX::XMFLOAT4 GetSpecularColor() const;

    void SetSpecularPower(float power);
    float GetSpecularPower() const;

private:
    ShaderType m_ShaderType;

    DirectX::XMFLOAT4 m_SpecularColor;
    float m_SpecularPower;
};