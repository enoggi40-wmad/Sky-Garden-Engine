#include "Material.h"

using namespace DirectX;

Material::Material()
    : m_ShaderType(ShaderType::Texture),
    m_SpecularColor(1.0f, 1.0f, 1.0f, 1.0f),
    m_SpecularPower(32.0f)
{
}

void Material::SetShaderType(ShaderType type)
{
    m_ShaderType = type;
}

ShaderType Material::GetShaderType() const
{
    return m_ShaderType;
}

void Material::SetSpecularColor(const XMFLOAT4& color)
{
    m_SpecularColor = color;
}

XMFLOAT4 Material::GetSpecularColor() const
{
    return m_SpecularColor;
}

void Material::SetSpecularPower(float power)
{
    m_SpecularPower = power;
}

float Material::GetSpecularPower() const
{
    return m_SpecularPower;
}