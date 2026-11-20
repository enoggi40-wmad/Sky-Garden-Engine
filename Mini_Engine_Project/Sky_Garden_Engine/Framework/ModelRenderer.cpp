#include "ModelRenderer.h"

#include "GameObject.h"
#include "modelclass.h"
#include "textureshaderclass.h"

#include "ShaderManager.h"
#include "Material.h"


// ModelRenderer 생성자
//
// ownerObject:
// 이 Component가 붙을 GameObject
//
// model:
// 이 GameObject가 사용할 실제 ModelClass
ModelRenderer::ModelRenderer(
    GameObject* ownerObject,
    ModelClass* model,
    Material* material)
    : Renderer(ownerObject),
    m_Model(model),
    m_Material(material),
    m_ShaderType(ShaderType::Texture)
{
}

void ModelRenderer::SetMaterial(Material* material)
{
    m_Material = material;
}

Material* ModelRenderer::GetMaterial() const
{
    return m_Material;
}


// 현재 연결되어 있는 ModelClass 반환
ModelClass* ModelRenderer::GetModel() const
{
    return m_Model;
}

void ModelRenderer::SetShaderType(ShaderType type)
{
    if (m_Material)
    {
        m_Material->SetShaderType(type);
    }
    else
    {
        m_ShaderType = type;
    }
}

ShaderType ModelRenderer::GetShaderType() const
{
    if (m_Material)
    {
        return m_Material->GetShaderType();
    }

    return m_ShaderType;
}

bool ModelRenderer::Render(
    const RenderContext& context)
{
    if (!m_Model || !m_Material)
    {
        return false;
    }

    // 이 ModelRenderer가 붙어있는 GameObject의
    // Transform을 이용해서 World Matrix를 만든다.
    //
    // position / rotation / scale 값이 모두 여기 반영된다.
    XMMATRIX worldMatrix =
        gameObject->transform.GetWorldMatrix();

    // Material에 텍스처가 있으면 우선 사용
    // 없으면 기존 ModelClass의 텍스처 사용

    ID3D11ShaderResourceView* diffuseTexture =
        m_Material ? m_Material->GetDiffuseTexture() : nullptr;

    ID3D11ShaderResourceView* secondTexture =
        m_Material ? m_Material->GetSecondTexture() : nullptr;
        
    ID3D11ShaderResourceView* alphaMap =
        m_Material ? m_Material->GetAlphaMap() : nullptr;
        

    ID3D11ShaderResourceView* normalMap =
        m_Material ? m_Material->GetNormalMap() : nullptr;
        

    // ModelClass가 가지고 있는 Vertex / Index Buffer를
    // Direct3D 렌더링 파이프라인에 준비한다.
    m_Model->Render(context.deviceContext);


    // Shader를 이용해서 실제 모델을 화면에 그린다.
    bool result = false;

    ShaderType shaderType = GetShaderType();

    if (shaderType == ShaderType::Texture)
    {
        result = context.shaderManager->GetTextureShader()->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            diffuseTexture
        );
    }

    else if (shaderType == ShaderType::Phong)
    {
        // Scene의 LightManager에서 수집한 광원 데이터
        const LightData& lightData = context.lightData;

        // 임시 Scene 공통 Ambient
        XMFLOAT4 ambientColor =
            context.ambientColor;

        // Material의 Specular 속성은 그대로 유지
        XMFLOAT4 specularColor =
            m_Material->GetSpecularColor();

        float specularPower =
            m_Material->GetSpecularPower();

        XMFLOAT4 emissiveColor =
            m_Material->GetEmissiveColor();

        float emissiveIntensity =
            m_Material->GetEmissiveIntensity();

        result = context.shaderManager->GetPhongShader()->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            diffuseTexture,
            context.cameraPosition,
            ambientColor,
            specularColor,
            specularPower,
            emissiveColor,
            emissiveIntensity,
            lightData

        );
    }

    else if (shaderType == ShaderType::AlphaMap)
    {
        // HLSL의 shaderTextures[0], [1], [2] 순서
        ID3D11ShaderResourceView* textures[3] =
        {
            diffuseTexture,       // 풀
            secondTexture,  // 흙
            alphaMap       // S자 마스크
        };

        auto* shader =
            context.shaderManager->GetAlphaMapShader();

        // 필요한 셰이더와 텍스처가 모두 준비됐는지 확인
        if (!shader)
        {
            OutputDebugStringA("[AlphaMap] Shader is null\n");
            return false;
        }

        if (!textures[0])
        {
            OutputDebugStringA("[AlphaMap] Grass texture is null\n");
            return false;
        }

        if (!textures[1])
        {
            OutputDebugStringA("[AlphaMap] Dirt texture is null\n");
            return false;
        }

        if (!textures[2])
        {
            OutputDebugStringA("[AlphaMap] Alpha map is null\n");
            return false;
        }

        result = shader->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            textures
        );


    }
    else if (shaderType == ShaderType::BumpMap)
    {
        // 사용할 셰이더 가져오기
        BumpMapShaderClass* shader =
            context.shaderManager->GetBumpMapShader();

        if (!shader)
        {
            return false;
        }

        // HLSL의 shaderTextures[0], shaderTextures[1]에 대응
        ID3D11ShaderResourceView* textures[2] =
        {
            diffuseTexture,
            normalMap
        };

        if (!textures[0] || !textures[1])
        {
            return false;
        }

        // 기존 Phong 분기와 같은 조명 설정
        XMFLOAT3 lightDirection =
        {
            0.0f, -1.0f, 1.0f
        };

        XMFLOAT4 diffuseColor =
        {
            0.2f, 0.2f, 0.2f, 1.0f
        };

        XMFLOAT4 ambientColor = context.ambientColor;

        XMFLOAT4 specularColor =
            m_Material->GetSpecularColor();

        float specularPower =
            m_Material->GetSpecularPower();

        // 노멀맵 + Phong 조명으로 렌더링
        result = shader->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            textures,
            lightDirection,
            diffuseColor,
            ambientColor,
            context.cameraPosition,
            specularColor,
            specularPower
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