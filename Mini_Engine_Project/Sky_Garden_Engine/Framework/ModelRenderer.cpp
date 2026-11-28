
#include "ModelRenderer.h"

#include "GameObject.h"
#include "modelclass.h"
#include "textureshaderclass.h"
#include "PhongShaderClass.h"
#include "ShaderManager.h"
#include "Material.h"

using namespace DirectX;

// =====================================================
// Constructor
// =====================================================

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

// =====================================================
// Material
// =====================================================

void ModelRenderer::SetMaterial(Material* material)
{
    m_Material = material;
}

Material* ModelRenderer::GetMaterial() const
{
    return m_Material;
}

// =====================================================
// Model
// =====================================================

ModelClass* ModelRenderer::GetModel() const
{
    return m_Model;
}

// =====================================================
// Shader Type
// =====================================================

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

// =====================================================
// Render
// =====================================================

bool ModelRenderer::Render(
    const RenderContext& context)
{
    if (!m_Model || !m_Material)
    {
        return false;
    }

    // =================================================
    // Transform
    // =================================================

    XMMATRIX worldMatrix =
        gameObject->transform.GetWorldMatrix();

    // =================================================
    // Material Textures
    // =================================================

    ID3D11ShaderResourceView* diffuseTexture =
        m_Material->GetDiffuseTexture();

    ID3D11ShaderResourceView* secondTexture =
        m_Material->GetSecondTexture();

    ID3D11ShaderResourceView* alphaMap =
        m_Material->GetAlphaMap();

    ID3D11ShaderResourceView* normalMap =
        m_Material->GetNormalMap();

    // 기본 텍스처 확인
    if (!diffuseTexture)
    {
        OutputDebugStringA(
            "[ModelRenderer] Diffuse Texture is null\n"
        );

        return false;
    }

    // =================================================
    // Vertex / Index Buffer
    // =================================================

    m_Model->Render(context.deviceContext);

    // =================================================
    // Shader Selection
    // =================================================

    ShaderType shaderType = GetShaderType();

    // =================================================
    // Texture Shader (Unlit)
    // =================================================

    if (shaderType == ShaderType::Texture)
    {
        auto* shader =
            context.shaderManager->GetTextureShader();

        if (!shader)
        {
            return false;
        }

        return shader->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            diffuseTexture
        );
    }

    // =================================================
    // Phong Shader (Lit)
    //
    // Diffuse Texture
    // Second Texture
    // Alpha Mapping
    // Normal Mapping
    // Emissive
    // =================================================

    else if (shaderType == ShaderType::Phong)
    {
        auto* shader =
            context.shaderManager->GetPhongShader();

        if (!shader)
        {
            return false;
        }

        // Scene Lighting
        const LightData& lightData =
            context.lightData;

        XMFLOAT4 ambientColor =
            context.ambientColor;

        // Material Specular
        XMFLOAT4 specularColor =
            m_Material->GetSpecularColor();

        float specularPower =
            m_Material->GetSpecularPower();

        // Material Emissive
        XMFLOAT4 emissiveColor =
            m_Material->GetEmissiveColor();

        float emissiveIntensity =
            m_Material->GetEmissiveIntensity();

        // 통합 Phong Shader
        return shader->Render(
            context.deviceContext,
            m_Model->GetIndexCount(),

            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,

            // Textures
            diffuseTexture,
            secondTexture,
            alphaMap,
            normalMap,

            // Camera
            context.cameraPosition,

            // Lighting
            ambientColor,
            specularColor,
            specularPower,

            // Emissive
            emissiveColor,
            emissiveIntensity,

            // Scene Lights
            lightData
        );
    }

    return false;
}
