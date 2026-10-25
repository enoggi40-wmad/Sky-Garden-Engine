#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class ShaderManager;

struct RenderContext
{
    ID3D11DeviceContext* deviceContext;

    ShaderManager* shaderManager;

    DirectX::XMMATRIX viewMatrix;
    DirectX::XMMATRIX projectionMatrix;
    DirectX::XMMATRIX orthoMatrix;

    DirectX::XMFLOAT3 cameraPosition;
};