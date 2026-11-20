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

void Material::SetDiffuseTexture(ID3D11ShaderResourceView* texture)
{
    m_DiffuseTexture = texture;
}

ID3D11ShaderResourceView* Material::GetDiffuseTexture() const
{
    return m_DiffuseTexture;
}

void Material::SetSecondTexture(ID3D11ShaderResourceView* texture)
{
    m_SecondTexture = texture;
}

ID3D11ShaderResourceView* Material::GetSecondTexture() const
{
    return m_SecondTexture;
}

void Material::SetAlphaMap(ID3D11ShaderResourceView* texture)
{
    m_AlphaMap = texture;
}

ID3D11ShaderResourceView* Material::GetAlphaMap() const
{
    return m_AlphaMap;
}

void Material::SetNormalMap(ID3D11ShaderResourceView* texture)
{
    m_NormalMap = texture;
}

ID3D11ShaderResourceView* Material::GetNormalMap() const
{
    return m_NormalMap;
}

void Material::SetEmissiveColor(
    const DirectX::XMFLOAT4& color)
{
    m_EmissiveColor = color;
}

DirectX::XMFLOAT4 Material::GetEmissiveColor() const
{
    return m_EmissiveColor;
}

void Material::SetEmissiveIntensity(float intensity)
{
    m_EmissiveIntensity = intensity;
}

float Material::GetEmissiveIntensity() const
{
    return m_EmissiveIntensity;
}


