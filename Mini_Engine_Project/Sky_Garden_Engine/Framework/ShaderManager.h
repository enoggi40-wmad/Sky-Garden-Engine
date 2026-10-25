#pragma once

#include <d3d11.h>
#include "textureshaderclass.h"
#include "PhongLightShaderClass.h"

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
    PhongLightShaderClass* GetPhongLightShader();


private:
    TextureShaderClass* m_TextureShader;
    PhongLightShaderClass* m_PhongLightShader;
};