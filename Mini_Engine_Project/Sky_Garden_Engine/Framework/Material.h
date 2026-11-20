#pragma once

#include "ShaderType.h"
#include <DirectXMath.h>

struct ID3D11ShaderResourceView;

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

    void SetDiffuseTexture(ID3D11ShaderResourceView* texture);
    ID3D11ShaderResourceView* GetDiffuseTexture() const;

	void SetSecondTexture(ID3D11ShaderResourceView* texture);
    ID3D11ShaderResourceView* GetSecondTexture() const;

    void SetAlphaMap(ID3D11ShaderResourceView* texture);
    ID3D11ShaderResourceView* GetAlphaMap() const;

    void SetNormalMap(ID3D11ShaderResourceView* texture);
	ID3D11ShaderResourceView* GetNormalMap() const;

private:
    ShaderType m_ShaderType;

    DirectX::XMFLOAT4 m_SpecularColor;
    float m_SpecularPower;

private:
    ID3D11ShaderResourceView* m_DiffuseTexture = nullptr;
    ID3D11ShaderResourceView* m_SecondTexture = nullptr;
    ID3D11ShaderResourceView* m_AlphaMap = nullptr;
    ID3D11ShaderResourceView* m_NormalMap = nullptr;

private:
    // 자체 발광 색상
    DirectX::XMFLOAT4 m_EmissiveColor =
    {
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // 자체 발광 강도
    float m_EmissiveIntensity = 0.0f;

public:
    void SetEmissiveColor(
        const DirectX::XMFLOAT4& color);

    DirectX::XMFLOAT4 GetEmissiveColor() const;

    void SetEmissiveIntensity(float intensity);

    float GetEmissiveIntensity() const;
};