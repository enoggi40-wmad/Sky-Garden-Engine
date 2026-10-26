#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
    m_TextureShader = nullptr;
    m_PhongLightShader = nullptr;
    m_SkyMapShader = 0;
}


ShaderManager::~ShaderManager()
{
}


bool ShaderManager::Initialize(
    ID3D11Device* device,
    HWND hwnd)
{
    // Texture Shader 생성
    m_TextureShader =
        new TextureShaderClass;

    if (!m_TextureShader)
    {
        return false;
    }


    // Texture Shader 초기화
    bool result =
        m_TextureShader->Initialize(
            device,
            hwnd
        );

    if (!result)
    {
        delete m_TextureShader;
        m_TextureShader = nullptr;

        return false;
    }


	// Phong Light Shader 생성
    m_PhongLightShader = new PhongLightShaderClass;
    if (!m_PhongLightShader)
    {
        return false;
    }

	// Phong Light Shader 초기화
    result = m_PhongLightShader->Initialize(device, hwnd);
    if (!result)
    {
        delete m_PhongLightShader;
        m_PhongLightShader = nullptr;

        return false;
    }

    

    // Sky Map Shader 생성
    m_SkyMapShader = new SkyMapShaderClass;

    if (!m_SkyMapShader)
    {
        return false;
    }

    // Sky Map Shader 초기화
    result = m_SkyMapShader->Initialize(
        device,
        hwnd
    );

    if (!result)
    {
        return false;
    }

    return true;
}


TextureShaderClass* ShaderManager::GetTextureShader()
{
    return m_TextureShader;
}

PhongLightShaderClass* ShaderManager::GetPhongLightShader()
{
    return m_PhongLightShader;
}

SkyMapShaderClass* ShaderManager::GetSkyMapShader()
{
    return m_SkyMapShader;
}

void ShaderManager::Shutdown()
{
    if (m_SkyMapShader)
    {
        m_SkyMapShader->Shutdown();

        delete m_SkyMapShader;
        m_SkyMapShader = 0;
    }

    if (m_PhongLightShader)
    {
        m_PhongLightShader->Shutdown();
        delete m_PhongLightShader;
        m_PhongLightShader = nullptr;
    }

    if (m_TextureShader)
    {
        m_TextureShader->Shutdown();

        delete m_TextureShader;
        m_TextureShader = nullptr;
    }
}