#pragma once

#include "Renderer.h"

class GameObject;
class ModelClass;

class BillboardRenderer : public Renderer
{
public:
    BillboardRenderer(GameObject* owner, ModelClass* model);
    ~BillboardRenderer();

    bool Render(
        ID3D11DeviceContext* deviceContext,
        TextureShaderClass* textureShader,
        DirectX::XMMATRIX viewMatrix,
        DirectX::XMMATRIX projectionMatrix,
		DirectX::XMFLOAT3 cameraPosition
    ) override;
private:
    ModelClass* m_Model;
};