#include "ModelRenderer.h"

#include "GameObject.h"
#include "modelclass.h"
#include "textureshaderclass.h"

#include "ShaderManager.h"


// ModelRenderer 생성자
//
// ownerObject:
// 이 Component가 붙을 GameObject
//
// model:
// 이 GameObject가 사용할 실제 ModelClass
ModelRenderer::ModelRenderer(
    GameObject* ownerObject,
    ModelClass* model)
    : Renderer(ownerObject), m_Model(model)
{
    m_Model = model;
    m_ShaderType = ShaderType::Texture;
    //m_ShaderType = ShaderType::PhongLight;
}


// 현재 연결되어 있는 ModelClass 반환
ModelClass* ModelRenderer::GetModel() const
{
    return m_Model;
}

// ModelRenderer가 사용할 ShaderType 설정
void ModelRenderer::SetShaderType(ShaderType type)
{
    m_ShaderType = type;
}

ShaderType ModelRenderer::GetShaderType() const
{
    return m_ShaderType;
}

bool ModelRenderer::Render(
    const RenderContext& context)
{
    if (!m_Model)
    {
        // ModelClass가 연결되어 있지 않으면 렌더링할 수 없음
        return false;
	}

    // 이 ModelRenderer가 붙어있는 GameObject의
    // Transform을 이용해서 World Matrix를 만든다.
    //
    // position / rotation / scale 값이 모두 여기 반영된다.
    XMMATRIX worldMatrix =
        gameObject->transform.GetWorldMatrix();


    // ModelClass가 가지고 있는 Vertex / Index Buffer를
    // Direct3D 렌더링 파이프라인에 준비한다.
    m_Model->Render(context.deviceContext);


    // Shader를 이용해서 실제 모델을 화면에 그린다.
    bool result = false;

    if (m_ShaderType == ShaderType::Texture)
    {
        result = context.shaderManager->GetTextureShader()->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            m_Model->GetTexture()
        );
    }
    else if (m_ShaderType == ShaderType::PhongLight)
    {
        XMFLOAT3 lightDirection =
        {
            0.0f, -1.0f, 1.0f
        };

        XMFLOAT4 ambientColor =
        {
            0.3f, 0.3f, 0.3f, 1.0f
        };

        XMFLOAT4 diffuseColor =
        {
            1.0f, 1.0f, 1.0f, 1.0f
        };

        XMFLOAT4 specularColor =
        {
            1.0f, 1.0f, 1.0f, 1.0f
        };

        float specularPower = 32.0f;

        XMFLOAT4 pointLightColor[2] =
        {
            XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
            XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
        };

        XMFLOAT4 pointLightPosition[2] =
        {
            XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f),
            XMFLOAT4(0.0f, 0.0f, 0.0f, 1.0f)
        };

        result = context.shaderManager->GetPhongLightShader()->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            m_Model->GetTexture(),
            lightDirection,
            ambientColor,
            diffuseColor,
            context.cameraPosition,
            specularColor,
            specularPower,
            pointLightColor,
            pointLightPosition
        );
    }

    // Shader 렌더링에 실패한 경우
    if (!result)
    {
        return false;
    }


    // 정상적으로 렌더링 완료
    return true;
}