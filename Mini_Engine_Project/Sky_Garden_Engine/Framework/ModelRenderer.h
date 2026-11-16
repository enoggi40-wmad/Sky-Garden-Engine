#pragma once

#include "Renderer.h"
#include "ShaderType.h"
#include <DirectXMath.h>

using namespace DirectX;

class TextureShaderClass;

struct ID3D11DeviceContext;

class ModelClass;

class Material;


class ModelRenderer : public Renderer
{
public:
    ModelRenderer(
        GameObject* ownerObject,
        ModelClass* model,
        Material* material = nullptr
    );

    virtual ~ModelRenderer() = default;

    ModelClass* GetModel() const;

    void SetMaterial(Material* material);
    Material* GetMaterial() const;

    bool Render(const RenderContext& context) override;

    void SetShaderType(ShaderType type);
    ShaderType GetShaderType() const;

private:
    ModelClass* m_Model;
    Material* m_Material;

    // 기존 코드 호환을 위해 임시 유지
    ShaderType m_ShaderType;
};