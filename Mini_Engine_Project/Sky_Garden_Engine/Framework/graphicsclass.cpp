////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "SceneManager.h"
#include "InputManager.h"
#include "Transform.h"

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

    // Text
    m_Text = nullptr;
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
    // Shader Manager
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
            L"Could not initialize the shader manager.",
            L"Error",
            MB_OK
        );

        return false;
    }

    
    m_PerformanceMonitor.Initialize(
        screenWidth,
        screenHeight
    );


    m_Text = new TextClass;

    if (!m_Text)
    {
        return false;
    }

    XMMATRIX baseViewMatrix =
        XMMatrixTranslation(0.0f, 0.0f, 1.0f);

    if (!m_Text->Initialize(
        m_D3D->GetDevice(),
        m_D3D->GetDeviceContext(),
        hwnd,
        screenWidth,
        screenHeight,
        baseViewMatrix))
    {
        return false;
    }




    return true;
}




bool GraphicsClass::Frame(int fps, int cpu)
{
    static auto previousTime =
        std::chrono::steady_clock::now();

    auto currentTime =
        std::chrono::steady_clock::now();

    std::chrono::duration<float> elapsedTime =
        currentTime - previousTime;

    float deltaTime = elapsedTime.count();

    previousTime = currentTime;

    // =====================================================
    // F3: Dirty Flag ON/OFF
    // =====================================================

    bool f3Pressed =
        (GetAsyncKeyState(VK_F3) & 0x8000) != 0;

    if (f3Pressed && !m_PreviousF3Pressed)
    {
        Transform::s_UseDirtyFlag =
            !Transform::s_UseDirtyFlag;
    }

    m_PreviousF3Pressed = f3Pressed;


    // =====================================================
    // F4: Performance Monitor ON/OFF
    // =====================================================

    bool f4Pressed =
        (GetAsyncKeyState(VK_F4) & 0x8000) != 0;

    if (f4Pressed && !m_PreviousF4Pressed)
    {
        m_ShowPerformanceMonitor =
            !m_ShowPerformanceMonitor;
    }

    m_PreviousF4Pressed = f4Pressed;


    // =====================================================
    // Performance Monitor Update
    // =====================================================

    m_PerformanceMonitor.Update(
        fps,
        cpu,
        deltaTime
    );

    m_PerformanceTextTimer += deltaTime;

    // 0.5초마다 텍스트 갱신
    if (m_PerformanceTextTimer >= 0.5f)
    {
        if (!m_Text->SetPerformanceInfo(
            m_PerformanceMonitor.GetFPS(),
            m_PerformanceMonitor.GetCPUUsage(),
            m_PerformanceMonitor.GetFrameTime(),
            m_PerformanceMonitor.GetMemoryUsageMB(),
            -1,  // Object Count
            -1,  // Polygon Count
            m_PerformanceMonitor.GetScreenWidth(),
            m_PerformanceMonitor.GetScreenHeight(),
            Transform::s_UseDirtyFlag,  // Dirty Flag ON/OFF
            m_D3D->GetDeviceContext()
        ))
        {
            return false;
        }

        m_PerformanceTextTimer = 0.0f;
    }


    // =====================================================
    // Scene Update
    // =====================================================

    m_SceneManager->Update(deltaTime);


    // =====================================================
    // Render
    // =====================================================

    if (!Render())
        return false;

    return true;
}




bool GraphicsClass::Render()
{
    XMMATRIX projectionMatrix;
    XMMATRIX orthoMatrix;


    // =====================================================
    // Scene Begin
    // =====================================================

    /*m_D3D->BeginScene(
        0.60f,
        0.82f,
        1.0f,
        1.0f
    );*/

    m_D3D->BeginScene(
        1.0f,
        0.0f,
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
            m_ShaderManager,
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
    // Performance Monitor Render
    // =====================================================

    if (m_ShowPerformanceMonitor &&
        m_Text &&
        !m_SceneManager->IsTitleScene())
    {
        XMMATRIX worldMatrix;
        m_D3D->GetWorldMatrix(worldMatrix);

        m_D3D->TurnZBufferOff();
        m_D3D->TurnOnAlphaBlending();

        bool textResult = m_Text->Render(
            m_D3D->GetDeviceContext(),
            worldMatrix,
            orthoMatrix
        );

        m_D3D->TurnOffAlphaBlending();
        m_D3D->TurnZBufferOn();

        if (!textResult)
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
	// Text
	// =====================================================

    if (m_Text)
    {
        m_Text->Shutdown();
        delete m_Text;
        m_Text = nullptr;
    }

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