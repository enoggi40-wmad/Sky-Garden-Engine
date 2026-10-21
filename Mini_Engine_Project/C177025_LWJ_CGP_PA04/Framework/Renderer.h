#pragma once

#include "Component.h"
#include <DirectXMath.h>
#include <d3d11.h>
#include "RenderContext.h"

class GameObject;
class TextureShaderClass;

class Renderer : public Component
{
public:
	Renderer(GameObject* owner);
	virtual ~Renderer();

    virtual bool Render(
        const RenderContext& context
    ) = 0;

public:
    void SetTransparent(bool transparent);
    bool IsTransparent() const;
    DirectX::XMFLOAT3 GetPosition() const;

protected:
    bool m_IsTransparent;

};