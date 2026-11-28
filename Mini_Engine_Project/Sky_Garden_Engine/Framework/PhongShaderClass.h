
#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: PhongShaderClass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PHONGSHADERCLASS_H_
#define _PHONGSHADERCLASS_H_

#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include <fstream>

#include "LightData.h"

class PhongShaderClass
{
private:
    struct MatrixBufferType
    {
        DirectX::XMMATRIX world;
        DirectX::XMMATRIX view;
        DirectX::XMMATRIX projection;
    };

    struct CameraBufferType
    {
        DirectX::XMFLOAT3 cameraPosition;
        float padding;
    };

    struct LightBufferType
    {
        DirectX::XMFLOAT4 ambientColor;
        DirectX::XMFLOAT4 specularColor;

        float specularPower;
        float padding[3];

        DirectX::XMFLOAT4 emissiveColor;

        float emissiveIntensity;
        float emissivePadding[3];
    };

    // PS b2: Mapping 활성화 여부
    struct TextureOptionBufferType
    {
        int useAlphaMap;
        int useNormalMap;
        float padding[2];
    };

public:
    PhongShaderClass();

    PhongShaderClass(const PhongShaderClass&) = delete;
    PhongShaderClass& operator=(const PhongShaderClass&) = delete;

    ~PhongShaderClass();

    bool Initialize(ID3D11Device* device, HWND hwnd);
    void Shutdown();

    bool Render(
        ID3D11DeviceContext* deviceContext,
        int indexCount,

        DirectX::XMMATRIX worldMatrix,
        DirectX::XMMATRIX viewMatrix,
        DirectX::XMMATRIX projectionMatrix,

        ID3D11ShaderResourceView* diffuseTexture,
        ID3D11ShaderResourceView* secondTexture,
        ID3D11ShaderResourceView* alphaMap,
        ID3D11ShaderResourceView* normalMap,

        DirectX::XMFLOAT3 cameraPosition,
        DirectX::XMFLOAT4 ambientColor,
        DirectX::XMFLOAT4 specularColor,
        float specularPower,

        DirectX::XMFLOAT4 emissiveColor,
        float emissiveIntensity,

        const LightData& lightData
    );

private:
    bool InitializeShader(
        ID3D11Device* device,
        HWND hwnd,
        const WCHAR* fileName
    );

    void ShutdownShader();

    void OutputShaderErrorMessage(
        ID3D10Blob* errorMessage,
        HWND hwnd,
        const WCHAR* shaderFilename
    );

    bool SetShaderParameters(
        ID3D11DeviceContext* deviceContext,

        DirectX::XMMATRIX worldMatrix,
        DirectX::XMMATRIX viewMatrix,
        DirectX::XMMATRIX projectionMatrix,

        ID3D11ShaderResourceView* diffuseTexture,
        ID3D11ShaderResourceView* secondTexture,
        ID3D11ShaderResourceView* alphaMap,
        ID3D11ShaderResourceView* normalMap,

        DirectX::XMFLOAT3 cameraPosition,
        DirectX::XMFLOAT4 ambientColor,
        DirectX::XMFLOAT4 specularColor,
        float specularPower,

        DirectX::XMFLOAT4 emissiveColor,
        float emissiveIntensity,

        const LightData& lightData
    );

    void RenderShader(
        ID3D11DeviceContext* deviceContext,
        int indexCount
    );

private:
    ID3D11VertexShader* m_vertexShader = nullptr;
    ID3D11PixelShader* m_pixelShader = nullptr;
    ID3D11InputLayout* m_layout = nullptr;
    ID3D11SamplerState* m_sampleState = nullptr;

    ID3D11Buffer* m_matrixBuffer = nullptr;
    ID3D11Buffer* m_cameraBuffer = nullptr;

    ID3D11Buffer* m_lightBuffer = nullptr;
    ID3D11Buffer* m_lightDataBuffer = nullptr;

    ID3D11Buffer* m_textureOptionBuffer = nullptr;
};

#endif
