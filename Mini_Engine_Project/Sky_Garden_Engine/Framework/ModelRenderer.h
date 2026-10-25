#pragma once

#include "Renderer.h"
#include <DirectXMath.h>

using namespace DirectX;

class TextureShaderClass;

struct ID3D11DeviceContext;

class ModelClass;

enum class ShaderType
{
    Texture,
    PhongLight
};


class ModelRenderer : public Renderer
{
public:

    
    ModelRenderer(GameObject* ownerObject, ModelClass* model);
       
    virtual ~ModelRenderer() = default;
       
    
    ModelClass* GetModel() const;
        
    bool Render(
        const RenderContext& context
    ) override;

    void SetShaderType(ShaderType type);
    ShaderType GetShaderType() const;

private:
        
    ModelClass* m_Model;
    ShaderType m_ShaderType;
};