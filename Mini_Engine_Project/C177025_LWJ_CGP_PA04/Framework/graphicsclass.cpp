////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"



GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	m_Billboard = 0;

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



    // =====================================================
    // Model Resources
    // =====================================================

    // Guardian Lion
    if (!m_ResourceManager->LoadModel(
        "GuardianLion",
        m_D3D->GetDevice(),
        L"./data/Guardian_Lion.obj",
        L"./data/Guardian_Lion.dds"
    ))
    {
        return false;
    }


    // Sakura Tree
    if (!m_ResourceManager->LoadModel(
        "SakuraTree",
        m_D3D->GetDevice(),
        L"./data/Sakura_Tree.obj",
        L"./data/Sakura_Tree.dds"
    ))
    {
        return false;
    }


    // Crystal Prism2
    if (!m_ResourceManager->LoadModel(
        "CrystalPrism2",
        m_D3D->GetDevice(),
        L"./data/Crystal_Prism2.obj",
        L"./data/Crystal_Prism2.dds"
    ))
    {
        return false;
    }


    // Water Plane
    if (!m_ResourceManager->LoadModel(
        "WaterPlane",
        m_D3D->GetDevice(),
        L"./data/WaterPlane.obj",
        L"./data/WaterPlane.dds"
    ))
    {
        return false;
    }


    // Floating Island
    if (!m_ResourceManager->LoadModel(
        "FloatingIsland",
        m_D3D->GetDevice(),
        L"./data/Floating_Island.obj",
        L"./data/Floating_Island.dds"
    ))
    {
        return false;
    }


    // Floating Alter
    if (!m_ResourceManager->LoadModel(
        "FloatingAlter",
        m_D3D->GetDevice(),
        L"./data/Floating_Alter.obj",
        L"./data/Floating_Alter.dds"
    ))
    {
        return false;
    }


    // Lantern
    if (!m_ResourceManager->LoadModel(
        "Lantern",
        m_D3D->GetDevice(),
        L"./data/Lantern.obj",
        L"./data/Lantern.dds"
    ))
    {
        return false;
    }


    // Golden Spire
    if (!m_ResourceManager->LoadModel(
        "GoldenSpire",
        m_D3D->GetDevice(),
        L"./data/Golden_Spire.obj",
        L"./data/Golden_Spire.dds"
    ))
    {
        return false;
    }


    // Circle Stone
    if (!m_ResourceManager->LoadModel(
        "CircleStone",
        m_D3D->GetDevice(),
        L"./data/Circle_Stone.obj",
        L"./data/Circle_Stone.dds"
    ))
    {
        return false;
    }


    // Crystal Prism
    if (!m_ResourceManager->LoadModel(
        "CrystalPrism",
        m_D3D->GetDevice(),
        L"./data/Crystal_Prism.obj",
        L"./data/Crystal_Prism.dds"
    ))
    {
        return false;
    }


    // Cloud
    if (!m_ResourceManager->LoadModel(
        "Cloud",
        m_D3D->GetDevice(),
        L"./data/Cloud.obj",
        L"./data/Cloud.dds"
    ))
    {
        return false;
    }

    // =====================================================
    // Billboard
    // =====================================================

    m_Billboard = new ModelClass;

    if (!m_Billboard)
    {
        return false;
    }

    result = m_Billboard->Initialize(
        m_D3D->GetDevice(),
        L"./data/Magic_Billboard.obj",
        L"./data/Magic_Billboard.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the billboard object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Scene
    // =====================================================
    //
    // Scene이 사용할 ModelClass들이 모두 초기화된 뒤
    // Scene과 GameObject들을 생성한다.
    //
    // ModelRenderer가 초기화되지 않은 ModelClass를
    // 참조하지 않도록 반드시 모델 로딩 이후에 실행한다.
    // =====================================================

    m_Scene = new Scene;

    if (!m_Scene)
    {
        return false;
    }


	// Scene에 필요한 GameObject들을 생성하고 구성
    // 기존 씬
    //m_Scene->Initialize(m_ResourceManager);

    // 과제용 씬
    m_Scene->InitializeAssignment(m_ResourceManager);



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


void GraphicsClass::Shutdown()
{
    // =====================================================
    // Scene
    // =====================================================
    //
    // Scene이 GameObject들을 소유하고 있으므로
    // Scene을 먼저 삭제한다.
    //
    // GameObject가 삭제되면서
    // ModelRenderer / Rotator / Orbiter / Floater 등의
    // Component들도 함께 삭제된다.
    // =====================================================

    // Scene 먼저 제거
    if (m_Scene)
    {
        delete m_Scene;
        m_Scene = nullptr;
    }

    // 모델 리소스는 ResourceManager가 한 번만 정리
    if (m_ResourceManager)
    {
        m_ResourceManager->Shutdown();
        delete m_ResourceManager;
        m_ResourceManager = nullptr;
    }


    if (m_Billboard)
    {
        m_Billboard->Shutdown();
        delete m_Billboard;
        m_Billboard = nullptr;
    }


    // 이후 Shader / Bitmap / Camera / D3D 등
    // 기존 리소스 정리 코드 유지
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

bool GraphicsClass::Frame(int fps, int cpu)
{
	bool result;
	static float rotation = 0.0f;

	// Title scene input.
	if (m_ShowTitleScene)
	{
		if (GetAsyncKeyState(VK_RETURN) & 0x8000)
		{
			m_ShowTitleScene = false;
		}
	}
	else
	{
		// Camera movement is only enabled in the main scene.
		m_Camera->UpdateFreeCamera(m_hwnd);
	}

	// Update the rotation variable each frame.
	rotation += (float)XM_PI * 0.005f;
	if (rotation > 360.0f)
	{
		rotation -= 360.0f;
	}

	int objectCount = 0;
	int polygonCount = 0;

	// Object count
	objectCount += 1;   // WaterPlane
	objectCount += 10;  // CrystalPrism
	objectCount += 4;   // CrystalPrism2
	objectCount += 4;   // Lantern
	objectCount += 12;  // Far Floating Islands
	objectCount += 8;   // Clouds
	objectCount += 1;   // FloatingAlter
	objectCount += 1;   // GoldenSpire
	objectCount += 1;   // CircleStone
	objectCount += 1;   // GuardianLion
	objectCount += 1;   // SakuraTree

	// Polygon count
    // Polygon count
    polygonCount +=
        m_ResourceManager->GetModel("WaterPlane")->GetIndexCount() / 3;

    polygonCount +=
        (m_ResourceManager->GetModel("CrystalPrism")->GetIndexCount() / 3) * 10;

    polygonCount +=
        (m_ResourceManager->GetModel("CrystalPrism2")->GetIndexCount() / 3) * 4;

    polygonCount +=
        (m_ResourceManager->GetModel("Lantern")->GetIndexCount() / 3) * 4;

    polygonCount +=
        (m_ResourceManager->GetModel("FloatingIsland")->GetIndexCount() / 3) * 12;

    polygonCount +=
        (m_ResourceManager->GetModel("Cloud")->GetIndexCount() / 3) * 8;

    polygonCount +=
        m_ResourceManager->GetModel("FloatingAlter")->GetIndexCount() / 3;

    polygonCount +=
        m_ResourceManager->GetModel("GoldenSpire")->GetIndexCount() / 3;

    polygonCount +=
        m_ResourceManager->GetModel("CircleStone")->GetIndexCount() / 3;

    polygonCount +=
        m_ResourceManager->GetModel("GuardianLion")->GetIndexCount() / 3;

    polygonCount +=
        m_ResourceManager->GetModel("SakuraTree")->GetIndexCount() / 3;

	result = m_Text->SetPerformanceInfo(
		fps,
		cpu,
		objectCount,
		polygonCount,
		800,
		600,
		m_D3D->GetDeviceContext());

	if (!result)
	{
		return false;
	}

	// Scene Update
	m_Scene->Update(0.016f);

	// Render the graphics scene.
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
    //m_D3D->BeginScene(0.60f, 0.82f, 1.0f, 1.0f);
    // 검정색 
    m_D3D->BeginScene(0.0f, 0.0f, 0.0f, 1.0f);


    // 현재 카메라 위치와 회전을 이용해 View Matrix 생성
    m_Camera->Render();


    // 렌더링에 필요한 Matrix 가져오기
    m_Camera->GetViewMatrix(viewMatrix);
    m_D3D->GetWorldMatrix(worldMatrix);
    m_D3D->GetProjectionMatrix(projectionMatrix);
    m_D3D->GetOrthoMatrix(orthoMatrix);



    // =====================================================
    // Title Scene
    // =====================================================

    if (m_ShowTitleScene)
    {
        // 2D 이미지이므로 ZBuffer 비활성화
        m_D3D->TurnZBufferOff();

        worldMatrix = XMMatrixIdentity();


        // Title Bitmap 렌더링 준비
        result = m_Bitmap->Render(
            m_D3D->GetDeviceContext(),
            0,
            0
        );

        if (!result)
        {
            MessageBox(
                NULL,
                L"Title Bitmap Render Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }


        // Title Texture 렌더링
        result = m_TextureShader->Render(
            m_D3D->GetDeviceContext(),
            m_Bitmap->GetIndexCount(),
            worldMatrix,
            m_BaseViewMatrix,
            orthoMatrix,
            m_Bitmap->GetTexture()
        );

        if (!result)
        {
            MessageBox(
                NULL,
                L"Title Texture Render Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }


        m_D3D->TurnZBufferOn();

        m_D3D->EndScene();

        return true;
    }

    // =====================================================
    // Scene GameObject 렌더링
    // =====================================================
    result = m_Scene->Render(
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
    // Billboard
    // =====================================================
    //
    // Billboard는 항상 카메라 방향을 바라봐야 하므로
    // 아직 GraphicsClass에서 직접 처리
    // =====================================================

    //m_D3D->TurnOnAlphaBlending();


    //// 현재 카메라의 Y축 회전값
    //float cameraYaw =
    //    m_Camera->GetRotationY();


    //worldMatrix =
    //    XMMatrixScaling(
    //        50.0f,
    //        50.0f,
    //        50.0f
    //    )
    //    *

    //    // 카메라를 바라보도록 회전
    //    XMMatrixRotationY(
    //        XMConvertToRadians(cameraYaw)
    //        + XM_PI
    //    )
    //    *

    //    XMMatrixTranslation(
    //        195.0f,
    //        30.0f,
    //        145.0f
    //    );


    //m_Billboard->Render(
    //    m_D3D->GetDeviceContext()
    //);


    //result = m_TextureShader->Render(
    //    m_D3D->GetDeviceContext(),
    //    m_Billboard->GetIndexCount(),
    //    worldMatrix,
    //    viewMatrix,
    //    projectionMatrix,
    //    m_Billboard->GetTexture()
    //);


    //if (!result)
    //{
    //    MessageBox(
    //        NULL,
    //        L"Billboard Render Failed",
    //        L"Render Error",
    //        MB_OK
    //    );

    //    return false;
    //}


    //m_D3D->TurnOffAlphaBlending();



    // =====================================================
    // 2D UI
    // =====================================================

    // UI는 깊이 검사를 사용하지 않음
    m_D3D->TurnZBufferOff();

    // Panel의 투명도 처리를 위해 Alpha Blending 활성화
    m_D3D->TurnOnAlphaBlending();


    worldMatrix = XMMatrixIdentity();


    // -----------------------------------------------------
    // Info Panel
    // -----------------------------------------------------

    /*result = m_InfoBitmap->Render(
        m_D3D->GetDeviceContext(),
        0,
        15
    );

    if (!result)
    {
        MessageBox(
            NULL,
            L"Info Bitmap Render Failed",
            L"Render Error",
            MB_OK
        );

        return false;
    }


    result = m_TextureShader->Render(
        m_D3D->GetDeviceContext(),
        m_InfoBitmap->GetIndexCount(),
        worldMatrix,
        m_BaseViewMatrix,
        orthoMatrix,
        m_InfoBitmap->GetTexture()
    );


    if (!result)
    {
        MessageBox(
            NULL,
            L"Info Bitmap Texture Render Failed",
            L"Render Error",
            MB_OK
        );

        return false;
    }*/



    // -----------------------------------------------------
    // Performance Text
    // -----------------------------------------------------

    /*worldMatrix = XMMatrixIdentity();


    result = m_Text->Render(
        m_D3D->GetDeviceContext(),
        worldMatrix,
        orthoMatrix
    );


    if (!result)
    {
        MessageBox(
            NULL,
            L"Text Render Failed",
            L"Render Error",
            MB_OK
        );

        return false;
    }*/


    // 렌더링 상태 복구
    m_D3D->TurnOffAlphaBlending();
    m_D3D->TurnZBufferOn();


    // 한 프레임 렌더링 종료
    m_D3D->EndScene();


    return true;
}