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
        const RenderContext& context
    ) override;
private:
    ModelClass* m_Model;
};