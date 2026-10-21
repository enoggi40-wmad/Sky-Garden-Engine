#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class TextureShaderClass;

struct RenderContext
{
    ID3D11DeviceContext* deviceContext;
    TextureShaderClass* textureShader;

    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projectionMatrix;
    DirectX::XMMATRIX orthoMatrix;

    DirectX::XMFLOAT3 cameraPosition;
};