////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"
#include "SceneManager.h"

#include <chrono>


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	//
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_InfoBitmap = 0;

	m_Text = 0;

	m_ShowTitleScene = false;

	m_hwnd = 0;

	
	

	// Scene
	m_Scene = nullptr;

    //Resource Manager
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

    m_hwnd = hwnd;


    // =====================================================
    // Direct3D
    // =====================================================

    // Direct3D 객체 생성
    m_D3D = new D3DClass;

    if (!m_D3D)
    {
        return false;
    }


    // Direct3D 초기화
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
    // Camera
    // =====================================================

    // Camera 객체 생성
    m_Camera = new CameraClass;

    if (!m_Camera)
    {
        return false;
    }


    // 초기 카메라 위치 / 회전
    m_Camera->SetPosition(
        0.0f,
        100.0f,
        0.0f
    );

    m_Camera->SetRotation(
        5.0f,
        0.0f,
        0.0f
    );



    // =====================================================
	// Resource Manager 생성
    // =====================================================

    m_ResourceManager = new ResourceManager;

    if (!m_ResourceManager)
    {
        return false;
    }

    m_ResourceManager->Initialize(
        m_D3D->GetDevice()
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

    m_TextureShader = new TextureShaderClass;

    if (!m_TextureShader)
    {
        return false;
    }


    result = m_TextureShader->Initialize(
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



    // =====================================================
    // Title Bitmap
    // =====================================================

    m_Bitmap = new BitmapClass;

    if (!m_Bitmap)
    {
        return false;
    }


    result = m_Bitmap->Initialize(
        m_D3D->GetDevice(),
        screenWidth,
        screenHeight,
        L"./data/Title.dds",
        800,
        600
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the bitmap object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Info Bitmap
    // =====================================================

    m_InfoBitmap = new BitmapClass;

    if (!m_InfoBitmap)
    {
        return false;
    }


    result = m_InfoBitmap->Initialize(
        m_D3D->GetDevice(),
        screenWidth,
        screenHeight,
        L"./data/Panel.dds",
        380,
        32
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the info bitmap object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // 2D UI Base View Matrix
    // =====================================================

    // 2D UI용 카메라 위치로 잠시 이동
    m_Camera->SetPosition(
        0.0f,
        0.0f,
        -1.0f
    );

    m_Camera->SetRotation(
        0.0f,
        0.0f,
        0.0f
    );


    // UI 렌더링에 사용할 View Matrix 저장
    m_Camera->Render();
    m_Camera->GetViewMatrix(
        m_BaseViewMatrix
    );


    // 메인 3D Scene의 시작 카메라 위치로 복구
    m_Camera->SetPosition(
        500.0f,
        500.0f,
        500.0f
    );

    m_Camera->SetRotation(
        45.0f,
        -135.0f,
        0.0f
    );



    // =====================================================
    // Text
    // =====================================================

    m_Text = new TextClass;

    if (!m_Text)
    {
        return false;
    }


    result = m_Text->Initialize(
        m_D3D->GetDevice(),
        m_D3D->GetDeviceContext(),
        hwnd,
        screenWidth,
        screenHeight,
        m_BaseViewMatrix
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the text object.",
            L"Error",
            MB_OK
        );

        return false;
    }


    return true;
}

///
bool GraphicsClass::Frame(int fps, int cpu)
{
    bool result;

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


    m_Camera->UpdateFreeCamera(
        m_hwnd,
        deltaTime
    );


    m_SceneManager->Update(
        deltaTime
    );


    result = Render();

    if (!result)
    {
        return false;
    }

    return true;
}

bool GraphicsClass::Render()
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
    bool result;


    // =====================================================
    // Scene 렌더링 준비
    // =====================================================

    // 화면 초기화
    // 하늘색
    m_D3D->BeginScene(0.60f, 0.82f, 1.0f, 1.0f);
    // 검정색 
    //_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);


    // 현재 카메라 위치와 회전을 이용해 View Matrix 생성
    m_Camera->Render();


    // 렌더링에 필요한 Matrix 가져오기
    m_Camera->GetViewMatrix(viewMatrix);
    m_D3D->GetWorldMatrix(worldMatrix);
    m_D3D->GetProjectionMatrix(projectionMatrix);
    m_D3D->GetOrthoMatrix(orthoMatrix);

    // =====================================================
	// SceneManager를 통해 현재 Scene 렌더링
    // =====================================================
    result = m_SceneManager->Render(
        m_D3D->GetDeviceContext(),
        m_TextureShader,
        viewMatrix,
        projectionMatrix
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
    // 2D UI
    // =====================================================

    // UI는 깊이 검사를 사용하지 않음
    m_D3D->TurnZBufferOff();

    // Panel의 투명도 처리를 위해 Alpha Blending 활성화
    m_D3D->TurnOnAlphaBlending();


    worldMatrix = XMMatrixIdentity();


    // 렌더링 상태 복구
    m_D3D->TurnOffAlphaBlending();
    m_D3D->TurnZBufferOn();


    // 한 프레임 렌더링 종료
    m_D3D->EndScene();


    return true;
}


void GraphicsClass::Shutdown()
{
    // Scene 제거
    if (m_Scene)
    {
        delete m_Scene;
        m_Scene = nullptr;
    }

	// SceneManager 제거
    if (m_SceneManager)
    {
        m_SceneManager->Shutdown();

        delete m_SceneManager;
        m_SceneManager = nullptr;
    }

    // 모델 리소스 제거
    if (m_ResourceManager)
    {
        m_ResourceManager->Shutdown();
        delete m_ResourceManager;
        m_ResourceManager = nullptr;
    }

    // 이후 Shader / Bitmap / Camera / D3D 등
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
    // Bitmap
    // =====================================================

    if (m_Bitmap)
    {
        m_Bitmap->Shutdown();
        delete m_Bitmap;
        m_Bitmap = nullptr;
    }

    if (m_InfoBitmap)
    {
        m_InfoBitmap->Shutdown();
        delete m_InfoBitmap;
        m_InfoBitmap = nullptr;
    }


    // =====================================================
    // Texture Shader
    // =====================================================

    if (m_TextureShader)
    {
        m_TextureShader->Shutdown();
        delete m_TextureShader;
        m_TextureShader = nullptr;
    }


    // =====================================================
    // Camera
    // =====================================================

    if (m_Camera)
    {
        delete m_Camera;
        m_Camera = nullptr;
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

