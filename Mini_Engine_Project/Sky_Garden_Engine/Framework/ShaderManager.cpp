#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
    m_TextureShader = nullptr;
    m_PhongLightShader = nullptr;
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

    return true;
}


TextureShaderClass*
ShaderManager::GetTextureShader()
{
    return m_TextureShader;
}

PhongLightShaderClass* ShaderManager::GetPhongLightShader()
{
    return m_PhongLightShader;
}


void ShaderManager::Shutdown()
{
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