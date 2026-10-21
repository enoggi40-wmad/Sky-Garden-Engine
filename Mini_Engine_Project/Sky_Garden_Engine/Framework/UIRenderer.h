#pragma once

#include "Renderer.h"

class GameObject;
class BitmapClass;

class UIRenderer : public Renderer
{
public:
    UIRenderer(
        GameObject* owner,
        BitmapClass* bitmap
    );
    ~UIRenderer();

    bool Render(
        const RenderContext& context
    ) override;

private:
    BitmapClass* m_Bitmap;
};