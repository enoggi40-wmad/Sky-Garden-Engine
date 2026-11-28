
#include "ShaderManager.h"

// =====================================================
// Constructor / Destructor
// =====================================================

ShaderManager::ShaderManager()
    : m_TextureShader(nullptr),
    m_PhongShader(nullptr),
    m_SkyMapShader(nullptr)
{
}

ShaderManager::~ShaderManager()
{
}

// =====================================================
// Initialize
// =====================================================

bool ShaderManager::Initialize(
    ID3D11Device* device,
    HWND hwnd)
{
    // =================================================
    // Texture Shader
    // =================================================

    m_TextureShader = new TextureShaderClass;

    if (!m_TextureShader->Initialize(device, hwnd))
    {
        Shutdown();
        return false;
    }

    // =================================================
    // Phong Shader
    //
    // Phong Lighting
    // Alpha Mapping
    // Normal Mapping
    // Emissive
    // =================================================

    m_PhongShader = new PhongShaderClass;

    if (!m_PhongShader->Initialize(device, hwnd))
    {
        Shutdown();
        return false;
    }

    // =================================================
    // Sky Map Shader
    // =================================================

    m_SkyMapShader = new SkyMapShaderClass;

    if (!m_SkyMapShader->Initialize(device, hwnd))
    {
        Shutdown();
        return false;
    }

    return true;
}

// =====================================================
// Get Shader
// =====================================================

TextureShaderClass* ShaderManager::GetTextureShader()
{
    return m_TextureShader;
}

PhongShaderClass* ShaderManager::GetPhongShader()
{
    return m_PhongShader;
}

SkyMapShaderClass* ShaderManager::GetSkyMapShader()
{
    return m_SkyMapShader;
}

// =====================================================
// Shutdown
// =====================================================

void ShaderManager::Shutdown()
{
    if (m_SkyMapShader)
    {
        m_SkyMapShader->Shutdown();

        delete m_SkyMapShader;
        m_SkyMapShader = nullptr;
    }

    if (m_PhongShader)
    {
        m_PhongShader->Shutdown();

        delete m_PhongShader;
        m_PhongShader = nullptr;
    }

    if (m_TextureShader)
    {
        m_TextureShader->Shutdown();

        delete m_TextureShader;
        m_TextureShader = nullptr;
    }
}
