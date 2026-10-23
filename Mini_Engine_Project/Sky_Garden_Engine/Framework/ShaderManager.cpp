#include "ShaderManager.h"


ShaderManager::ShaderManager()
{
    m_TextureShader = nullptr;
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


    return true;
}


TextureShaderClass*
ShaderManager::GetTextureShader()
{
    return m_TextureShader;
}


void ShaderManager::Shutdown()
{
    if (m_TextureShader)
    {
        m_TextureShader->Shutdown();

        delete m_TextureShader;
        m_TextureShader = nullptr;
    }
}