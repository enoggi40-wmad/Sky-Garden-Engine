#include "BillboardRenderer.h"
#include "GameObject.h"
#include "ModelClass.h"
#include "TextureShaderClass.h"
#include <cmath>

#include "ShaderManager.h"

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
    const RenderContext& context)
{
    if (!m_Model)
    {
        return false;
    }

    // GameObject의 로컬 위치 가져오기
    XMFLOAT3 position =
        gameObject->transform.GetLocalPosition();

    // 카메라를 바라보는 Y축 회전각 계산
    float angle =
        atan2f(
            context.cameraPosition.x - position.x,
            context.cameraPosition.z - position.z
        );

    // GameObject의 로컬 크기 가져오기
    XMFLOAT3 scale =
        gameObject->transform.GetLocalScale();

    // 크기 행렬 생성
    XMMATRIX scaleMatrix =
        XMMatrixScaling(
            scale.x,
            scale.y,
            scale.z
        );

    // 카메라를 향하도록 Y축 회전
    XMMATRIX rotationMatrix =
        XMMatrixRotationY(angle);

    // 위치 행렬 생성
    XMMATRIX translationMatrix =
        XMMatrixTranslation(
            position.x,
            position.y,
            position.z
        );

    // 최종 World Matrix 계산
    XMMATRIX worldMatrix =
        scaleMatrix *
        rotationMatrix *
        translationMatrix;

    // 모델의 Vertex / Index Buffer 설정
    m_Model->Render(context.deviceContext);

    // Shader를 사용해 모델 렌더링
    bool result =
        context.shaderManager->GetTextureShader()->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            m_Model->GetTexture()
        );

    if (!result)
    {
        return false;
    }

    return true;
}
