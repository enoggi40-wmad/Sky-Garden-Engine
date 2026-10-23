#pragma once

#include <d3d11.h>
#include "textureshaderclass.h"


class ShaderManager
{
public:
    ShaderManager();
    ~ShaderManager();

    bool Initialize(
        ID3D11Device* device,
        HWND hwnd
    );

    void Shutdown();

    TextureShaderClass* GetTextureShader();


private:
    TextureShaderClass* m_TextureShader;
};