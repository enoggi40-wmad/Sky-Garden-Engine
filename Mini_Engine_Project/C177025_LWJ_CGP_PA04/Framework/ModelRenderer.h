#pragma once

#include "Renderer.h"
#include <DirectXMath.h>

using namespace DirectX;

class TextureShaderClass;

struct ID3D11DeviceContext;

class ModelClass;


class ModelRenderer : public Renderer
{
public:

    
    ModelRenderer(GameObject* ownerObject, ModelClass* model);
       
    virtual ~ModelRenderer() = default;
       
    
    ModelClass* GetModel() const;
        
    bool Render(
        const RenderContext& context
    ) override;


private:

    // 실제 모델 데이터를 가지고 있는 ModelClass의 주소
    ModelClass* m_Model;
};