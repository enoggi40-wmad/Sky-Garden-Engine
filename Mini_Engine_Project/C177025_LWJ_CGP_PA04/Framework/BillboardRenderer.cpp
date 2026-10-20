#include "BillboardRenderer.h"
#include "GameObject.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include <cmath>

using namespace DirectX;


BillboardRenderer::BillboardRenderer(GameObject* owner,ModelClass* model)
    : Renderer(owner)
{
    m_Model = model;

    SetTransparent(true);
}

BillboardRenderer::~BillboardRenderer()
{
}

bool BillboardRenderer::Render(
    ID3D11DeviceContext* deviceContext,
    TextureShaderClass* textureShader,
    DirectX::XMMATRIX viewMatrix,
    DirectX::XMMATRIX projectionMatrix,
    DirectX::XMFLOAT3 cameraPosition)
{
    if(!m_Model)
    {
        return false;
	}

	// World Matrix °è»ê
    XMFLOAT3 position =
        gameObject->transform.position;

    float angle =
        atan2f(
            cameraPosition.x - position.x,
            cameraPosition.z - position.z
        );

    XMMATRIX scaleMatrix =
        XMMatrixScaling(
            gameObject->transform.scale.x,
            gameObject->transform.scale.y,
            gameObject->transform.scale.z
        );

    XMMATRIX rotationMatrix =
        XMMatrixRotationY(angle);

    XMMATRIX translationMatrix =
        XMMatrixTranslation(
            position.x,
            position.y,
            position.z
        );

    XMMATRIX worldMatrix =
        scaleMatrix *
        rotationMatrix *
        translationMatrix;

    // ¸ðµ¨ÀÇ Vertex / Index Buffer ¼³Á¤
    m_Model->Render(deviceContext);

    // Shader¸¦ »ç¿ëÇØ ¸ðµ¨ ·»´õ¸µ
    bool result = textureShader->Render(
        deviceContext,
        m_Model->GetIndexCount(),
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        m_Model->GetTexture()
    );

    if (!result)
    {
        return false;
    }

    return true;
}