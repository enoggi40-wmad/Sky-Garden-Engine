////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "SceneManager.h"
#include "InputManager.h"

#include <chrono>


GraphicsClass::GraphicsClass()
{
    m_D3D = nullptr;

    // ShaderManager
    m_ShaderManager = nullptr;

    // Scene Manager
    m_SceneManager = nullptr;

    // Resource Manager
    m_ResourceManager = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(
    int screenWidth,
    int screenHeight,
    HWND hwnd)
{
    bool result;

    InputManager::SetWindowHandle(hwnd);


    // =====================================================
    // Direct3D
    // =====================================================

    m_D3D = new D3DClass;

    if (!m_D3D)
    {
        return false;
    }


    result = m_D3D->Initialize(
        screenWidth,
        screenHeight,
        VSYNC_ENABLED,
        hwnd,
        FULL_SCREEN,
        SCREEN_DEPTH,
        SCREEN_NEAR
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize Direct3D.",
            L"Error",
            MB_OK
        );

        return false;
    }


    // =====================================================
    // Resource Manager
    // =====================================================

    m_ResourceManager = new ResourceManager;

    if (!m_ResourceManager)
    {
        return false;
    }


    m_ResourceManager->Initialize(
        m_D3D->GetDevice(),
        screenWidth,
        screenHeight
    );


    // =====================================================
    // Scene Manager
    // =====================================================

    m_SceneManager = new SceneManager;

    if (!m_SceneManager)
    {
        return false;
    }


    if (!m_SceneManager->Initialize(
        m_ResourceManager))
    {
        return false;
    }


    // =====================================================
    // Texture Shader
    // =====================================================

    m_ShaderManager = new ShaderManager;

    if (!m_ShaderManager)
    {
        return false;
    }


    result = m_ShaderManager->Initialize(
        m_D3D->GetDevice(),
        hwnd
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the texture shader object.",
            L"Error",
            MB_OK
        );

        return false;
    }


    return true;
}


bool GraphicsClass::Frame(int fps, int cpu)
{
    static auto previousTime =
        std::chrono::high_resolution_clock::now();

    auto currentTime =
        std::chrono::high_resolution_clock::now();

    std::chrono::duration<float> elapsedTime =
        currentTime - previousTime;

    float deltaTime =
        elapsedTime.count();

    previousTime =
        currentTime;


    m_SceneManager->Update(
        deltaTime
    );


    bool result = Render();

    if (!result)
    {
        return false;
    }


    return true;
}


bool GraphicsClass::Render()
{
    XMMATRIX projectionMatrix;
    XMMATRIX orthoMatrix;


    // =====================================================
    // Scene Begin
    // =====================================================

    m_D3D->BeginScene(
        0.60f,
        0.82f,
        1.0f,
        1.0f
    );


    // =====================================================
    // Render Matrix
    // =====================================================

    m_D3D->GetProjectionMatrix(
        projectionMatrix
    );

    m_D3D->GetOrthoMatrix(
        orthoMatrix
    );


    // =====================================================
    // Scene Render
    // =====================================================

    bool result =
        m_SceneManager->Render(
            m_D3D,
            m_D3D->GetDeviceContext(),
            m_ShaderManager->GetTextureShader(),
            projectionMatrix,
            orthoMatrix
        );

    if (!result)
    {
        MessageBox(
            NULL,
            L"Scene Render Failed",
            L"Render Error",
            MB_OK
        );

        return false;
    }


    // =====================================================
    // Scene End
    // =====================================================

    m_D3D->EndScene();


    return true;
}


void GraphicsClass::Shutdown()
{
    // =====================================================
    // Scene Manager
    // =====================================================

    if (m_SceneManager)
    {
        m_SceneManager->Shutdown();

        delete m_SceneManager;
        m_SceneManager = nullptr;
    }


    // =====================================================
    // Resource Manager
    // =====================================================

    if (m_ResourceManager)
    {
        m_ResourceManager->Shutdown();

        delete m_ResourceManager;
        m_ResourceManager = nullptr;
    }


    // =====================================================
    // ShaderManager
    // =====================================================

    if (m_ShaderManager)
    {
        m_ShaderManager->Shutdown();

        delete m_ShaderManager;
        m_ShaderManager = nullptr;
    }


    // =====================================================
    // Direct3D
    // =====================================================

    if (m_D3D)
    {
        m_D3D->Shutdown();

        delete m_D3D;
        m_D3D = nullptr;
    }
}