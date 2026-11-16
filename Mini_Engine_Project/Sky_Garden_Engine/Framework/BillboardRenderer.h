#pragma once

#include "Renderer.h"

class GameObject;
class ModelClass;
class Material;

class BillboardRenderer : public Renderer
{
public:
    BillboardRenderer(GameObject* owner, ModelClass* model, Material* material);
    ~BillboardRenderer();

    bool Render(
        const RenderContext& context
    ) override;
private:
    ModelClass* m_Model;
    Material* m_Material = nullptr;
};