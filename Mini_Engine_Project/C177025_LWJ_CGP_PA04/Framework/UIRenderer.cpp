#include "UIRenderer.h"
#include "bitmapclass.h"
#include "textureshaderclass.h"
#include <DirectXMath.h>

using namespace DirectX;

UIRenderer::UIRenderer(GameObject* owner, BitmapClass* bitmap)
    : Renderer(owner)
{
	m_Bitmap = bitmap;
}

UIRenderer::~UIRenderer()
{
}

bool UIRenderer::Render(
    const RenderContext& context)
{
    if(!m_Bitmap)
    {
        return false;
	}

    // UI Bitmap의 화면 위치에 맞게
	// Vertex Buffer를 업데이트
    bool result = m_Bitmap->Render(
        context.deviceContext,
		0, 0
    );
    if (!result)
    {
        MessageBox(
            NULL,
            L"Bitmap Render Failed",
            L"Error",
            MB_OK
        );

        return false;
    }

    // UI는 별도의 3D Transform을 사용하지 않으므로
    // World Matrix는 Identity Matrix를 사용한다.
    XMMATRIX worldMatrix =
        XMMatrixIdentity();

    XMMATRIX uiViewMatrix =
        XMMatrixTranslation(
            0.0f,
            0.0f,
            1.0f
        );

    // UI는 일반 3D Projection 대신
    // Ortho Matrix를 사용해 화면에 출력한다.
    result = context.textureShader->Render(
        context.deviceContext,
        m_Bitmap->GetIndexCount(),
        worldMatrix,
        uiViewMatrix,
        context.orthoMatrix,
        m_Bitmap->GetTexture()
    );

    if (!result)
    {
        return false;
    }

    return true;
}