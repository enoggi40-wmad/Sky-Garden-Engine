#pragma once

#include <d3d11.h>
#include "textureshaderclass.h"
#include "PhongShaderClass.h"
#include "SkyMapShaderClass.h"
#include "AlphaMapShaderClass.h"
#include "BumpMapShaderClass.h"

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
    PhongShaderClass* GetPhongShader();
    SkyMapShaderClass* GetSkyMapShader();
	AlphaMapShaderClass* GetAlphaMapShader();
    BumpMapShaderClass* GetBumpMapShader();


private:
    TextureShaderClass* m_TextureShader;
    PhongShaderClass* m_PhongShader;
    SkyMapShaderClass* m_SkyMapShader;
	AlphaMapShaderClass* m_AlphaMapShader;
    BumpMapShaderClass* m_BumpMapShader;
};