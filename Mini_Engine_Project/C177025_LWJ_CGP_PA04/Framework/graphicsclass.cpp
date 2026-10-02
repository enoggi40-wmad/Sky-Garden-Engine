////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "graphicsclass.h"


GraphicsClass::GraphicsClass()
{
	m_D3D = 0;
	m_Camera = 0;

	// Models
	m_WaterPlane = 0;

	m_FloatingIsland = 0;
	m_FloatingAlter = 0;
	m_GuardianLion = 0;
	m_CrystalPrism = 0;
	m_Cloud = 0;
	m_SakuraTree = 0;
	m_CrystalPrism2 = 0;
	m_CircleStone = 0;
	m_GoldenSpire = 0;
	m_Lantern = 0;

	m_Billboard = 0;

	//
	m_TextureShader = 0;
	m_Bitmap = 0;
	m_InfoBitmap = 0;

	m_Text = 0;

	m_ShowTitleScene = true;

	m_hwnd = 0;

	
	

	// Scene
	m_Scene = nullptr;
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
    // Model Resources
    // =====================================================
    //
    // GraphicsClass는 아직 ModelClass 리소스의
    // 생성과 초기화를 담당한다.
    //
    // 모든 ModelClass 초기화가 끝난 뒤
    // Scene::Initialize()를 호출한다.
    // =====================================================



    // =====================================================
    // Guardian Lion
    // =====================================================

    m_GuardianLion = new ModelClass;

    if (!m_GuardianLion)
    {
        return false;
    }


    result = m_GuardianLion->Initialize(
        m_D3D->GetDevice(),
        L"./data/Guardian_Lion.obj",
        L"./data/Guardian_Lion.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the guardian lion object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Sakura Tree
    // =====================================================

    m_SakuraTree = new ModelClass;

    if (!m_SakuraTree)
    {
        return false;
    }


    result = m_SakuraTree->Initialize(
        m_D3D->GetDevice(),
        L"./data/Sakura_Tree.obj",
        L"./data/Sakura_Tree.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the SakuraTree object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Crystal Prism2
    // =====================================================

    m_CrystalPrism2 = new ModelClass;

    if (!m_CrystalPrism2)
    {
        return false;
    }


    result = m_CrystalPrism2->Initialize(
        m_D3D->GetDevice(),
        L"./data/Crystal_Prism2.obj",
        L"./data/Crystal_Prism2.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the crystal prism2 object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Water Plane
    // =====================================================

    m_WaterPlane = new ModelClass;

    if (!m_WaterPlane)
    {
        return false;
    }


    result = m_WaterPlane->Initialize(
        m_D3D->GetDevice(),
        L"./data/WaterPlane.obj",
        L"./data/WaterPlane.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the water plane object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Floating Island
    // =====================================================

    m_FloatingIsland = new ModelClass;

    if (!m_FloatingIsland)
    {
        return false;
    }


    result = m_FloatingIsland->Initialize(
        m_D3D->GetDevice(),
        L"./data/Floating_Island.obj",
        L"./data/Floating_Island.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the floating island object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Floating Alter
    // =====================================================

    m_FloatingAlter = new ModelClass;

    if (!m_FloatingAlter)
    {
        return false;
    }


    result = m_FloatingAlter->Initialize(
        m_D3D->GetDevice(),
        L"./data/Floating_Alter.obj",
        L"./data/Floating_Alter.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the floating alter object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Lantern
    // =====================================================

    m_Lantern = new ModelClass;

    if (!m_Lantern)
    {
        return false;
    }


    result = m_Lantern->Initialize(
        m_D3D->GetDevice(),
        L"./data/Lantern.obj",
        L"./data/Lantern.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the lantern object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Golden Spire
    // =====================================================

    m_GoldenSpire = new ModelClass;

    if (!m_GoldenSpire)
    {
        return false;
    }


    result = m_GoldenSpire->Initialize(
        m_D3D->GetDevice(),
        L"./data/Golden_Spire.obj",
        L"./data/Golden_Spire.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the GoldenSpire object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Circle Stone
    // =====================================================

    m_CircleStone = new ModelClass;

    if (!m_CircleStone)
    {
        return false;
    }


    result = m_CircleStone->Initialize(
        m_D3D->GetDevice(),
        L"./data/Circle_Stone.obj",
        L"./data/Circle_Stone.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the CircleStone object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Crystal Prism
    // =====================================================

    m_CrystalPrism = new ModelClass;

    if (!m_CrystalPrism)
    {
        return false;
    }


    result = m_CrystalPrism->Initialize(
        m_D3D->GetDevice(),
        L"./data/Crystal_Prism.obj",
        L"./data/Crystal_Prism.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the crystal prism object.",
            L"Error",
            MB_OK
        );

        return false;
    }



    // =====================================================
    // Cloud
    // =====================================================

    m_Cloud = new ModelClass;

    if (!m_Cloud)
    {
        return false;
    }


    result = m_Cloud->Initialize(
        m_D3D->GetDevice(),
        L"./data/Cloud.obj",
        L"./data/Cloud.dds"
    );

    if (!result)
    {
        MessageBox(
            hwnd,
            L"Could not initialize the Cloud object.",
            L"Error",
            MB_OK
        );

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


    // Scene 내부에서 GameObject 생성,
    // Transform 설정, Component 부착을 수행한다.
    m_Scene->Initialize(
        m_GuardianLion,
        m_SakuraTree,
        m_WaterPlane,
        m_FloatingAlter,
        m_GoldenSpire,
        m_CircleStone
    );



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
	/////////////////////////////////////////////////////////////////////////
	// Release the model object.

	// Scene 삭제
	if (m_Scene)
	{
		delete m_Scene;
		m_Scene = nullptr;	
	}

	// 그 다음 실제 Model 리소스 삭제 (추후 ResourceManager로 옮길 예정)
	if (m_GuardianLion)
	{
		m_GuardianLion->Shutdown();
		delete m_GuardianLion;
		m_GuardianLion = nullptr;
	}

	if (m_SakuraTree)
	{
		m_SakuraTree->Shutdown();
		delete m_SakuraTree;
		m_SakuraTree = nullptr;
	}
	/////////////////////////////////////////////////////////////////////////

	if (m_WaterPlane)
	{
		m_WaterPlane->Shutdown();
		delete m_WaterPlane;
		m_WaterPlane = 0;
	}

	if (m_CircleStone)
	{
		m_CircleStone->Shutdown();
		delete m_CircleStone;
		m_CircleStone = 0;
	}

	if (m_Lantern)
	{
		m_Lantern->Shutdown();
		delete m_Lantern;
		m_Lantern = 0;
	}

	if (m_GoldenSpire)
	{
		m_GoldenSpire->Shutdown();
		delete m_GoldenSpire;
		m_GoldenSpire = 0;
	}

	if (m_CrystalPrism)
	{
		m_CrystalPrism->Shutdown();
		delete m_CrystalPrism;
		m_CrystalPrism = 0;
	}

	if (m_FloatingAlter)
	{
		m_FloatingAlter->Shutdown();
		delete m_FloatingAlter;
		m_FloatingAlter = 0;
	}

	if (m_Cloud)
	{
		m_Cloud->Shutdown();
		delete m_Cloud;
		m_Cloud = 0;
	}

	if (m_FloatingIsland)
	{
		m_FloatingIsland->Shutdown();
		delete m_FloatingIsland;
		m_FloatingIsland = 0;
	}

	if (m_CrystalPrism2)
	{
		m_CrystalPrism2->Shutdown();
		delete m_CrystalPrism2;
		m_CrystalPrism2 = 0;
	}

	if (m_Billboard)
	{
		m_Billboard->Shutdown();
		delete m_Billboard;
		m_Billboard = 0;
	}

	
	///////////////////////////////////////////////////////////////////////////

	// Release the camera object.
	if(m_Camera)
	{
		delete m_Camera;
		m_Camera = 0;
	}

	// Release the D3D object.
	if(m_D3D)
	{
		m_D3D->Shutdown();
		delete m_D3D;
		m_D3D = 0;
	}

	// Release the bitmap object.
	if (m_Bitmap)
	{
		m_Bitmap->Shutdown();
		delete m_Bitmap;
		m_Bitmap = 0;
	}

	if (m_InfoBitmap)
	{
		m_InfoBitmap->Shutdown();
		delete m_InfoBitmap;
		m_InfoBitmap = 0;
	}

	// Release the texture shader object.
	if (m_TextureShader)
	{
		m_TextureShader->Shutdown();
		delete m_TextureShader;
		m_TextureShader = 0;
	}

	return;
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
	polygonCount += m_WaterPlane->GetIndexCount() / 3;
	polygonCount += (m_CrystalPrism->GetIndexCount() / 3) * 10;
	polygonCount += (m_CrystalPrism2->GetIndexCount() / 3) * 4;
	polygonCount += (m_Lantern->GetIndexCount() / 3) * 4;
	polygonCount += (m_FloatingIsland->GetIndexCount() / 3) * 12;
	polygonCount += (m_Cloud->GetIndexCount() / 3) * 8;
	polygonCount += m_FloatingAlter->GetIndexCount() / 3;
	polygonCount += m_GoldenSpire->GetIndexCount() / 3;
	polygonCount += m_CircleStone->GetIndexCount() / 3;
	polygonCount += m_GuardianLion->GetIndexCount() / 3;
	polygonCount += m_SakuraTree->GetIndexCount() / 3;

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
	result = Render(rotation);
	if (!result)
	{
		return false;
	}

	return true;
}

bool GraphicsClass::Render(float rotation)
{
    XMMATRIX worldMatrix, viewMatrix, projectionMatrix, orthoMatrix;
    bool result;


    // =====================================================
    // Scene 렌더링 준비
    // =====================================================

    // 화면 초기화
    m_D3D->BeginScene(0.60f, 0.82f, 1.0f, 1.0f);


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
    //
    // Scene에 등록된 GameObject들의 ModelRenderer를 찾아 렌더링한다.
    //
    // 현재 Scene에서 관리하는 오브젝트:
    // - GuardianLion
    // - SakuraTree
    // - WaterPlane
    // - FloatingAlter
    // - GoldenSpire
    // - CircleStone
    //
    // 위 오브젝트들은 GraphicsClass에서 직접 렌더링하지 않는다.
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
    // Crystal Prism
    // =====================================================
    //
    // 아직 움직임 로직이 GraphicsClass에 있으므로
    // Scene으로 이동하지 않고 기존 방식 유지
    // =====================================================

    const int iscrystalCount = 10;
    const float radius = 700.0f;

    const float centerX = 0.0f;
    const float centerY = -300.0f;
    const float centerZ = 0.0f;

    const float orbitSpeed = 0.03f;  // 공전 속도
    const float selfSpeed = 1.0f;    // 자전 속도


    for (int i = 0; i < iscrystalCount; i++)
    {
        // 각 Crystal의 기본 배치 각도
        float baseAngle =
            (XM_2PI / iscrystalCount) * i;


        // 시간에 따라 공전
        float orbitAngle =
            baseAngle + rotation * orbitSpeed;


        // 원형 위치 계산
        float x =
            centerX + cosf(orbitAngle) * radius;

        float z =
            centerZ + sinf(orbitAngle) * radius;


        worldMatrix =
            XMMatrixScaling(
                50.0f,
                50.0f,
                50.0f
            )
            *

            // 공전 방향에 맞춘 회전 + 자전
            XMMatrixRotationY(
                -orbitAngle
                + XM_PIDIV2
                + rotation * selfSpeed
            )
            *

            XMMatrixTranslation(
                x,
                centerY,
                z
            );


        m_CrystalPrism->Render(
            m_D3D->GetDeviceContext()
        );


        result = m_TextureShader->Render(
            m_D3D->GetDeviceContext(),
            m_CrystalPrism->GetIndexCount(),
            worldMatrix,
            viewMatrix,
            projectionMatrix,
            m_CrystalPrism->GetTexture()
        );


        if (!result)
        {
            MessageBox(
                NULL,
                L"CrystalPrism Instance Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }
    }



    // =====================================================
    // Crystal Prism2
    // =====================================================
    //
    // 4개의 수정이 각 위치에서 상하 이동 및 회전
    // =====================================================

    const int yellowCrystalCount = 4;


    XMFLOAT3 yellowCrystalPositions[yellowCrystalCount] =
    {
        XMFLOAT3(-550.0f, 120.0f,  550.0f),
        XMFLOAT3(550.0f, 120.0f,  550.0f),
        XMFLOAT3(-550.0f, 120.0f, -550.0f),
        XMFLOAT3(550.0f, 120.0f, -550.0f)
    };


    for (int i = 0; i < yellowCrystalCount; i++)
    {
        // Sin 값을 이용한 상하 이동
        float floatOffsetY =
            sinf(rotation * 0.5f + i * 2.0f)
            * 10.0f;


        worldMatrix =
            XMMatrixScaling(
                50.0f,
                50.0f,
                50.0f
            )
            *
            XMMatrixRotationY(
                rotation * 0.5f + i
            )
            *
            XMMatrixTranslation(
                yellowCrystalPositions[i].x,
                yellowCrystalPositions[i].y + floatOffsetY,
                yellowCrystalPositions[i].z
            );


        m_CrystalPrism2->Render(
            m_D3D->GetDeviceContext()
        );


        result = m_TextureShader->Render(
            m_D3D->GetDeviceContext(),
            m_CrystalPrism2->GetIndexCount(),
            worldMatrix,
            viewMatrix,
            projectionMatrix,
            m_CrystalPrism2->GetTexture()
        );


        if (!result)
        {
            MessageBox(
                NULL,
                L"Yellow Crystal Render Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }
    }



    // =====================================================
    // Lantern
    // =====================================================
    //
    // 4개의 랜턴이 상하 이동 및 회전
    // =====================================================

    const int LanternCount = 4;


    XMFLOAT3 LanternPositions[LanternCount] =
    {
        XMFLOAT3(-150.0f, -300.0f,  150.0f),
        XMFLOAT3(150.0f, -300.0f,  150.0f),
        XMFLOAT3(-150.0f, -300.0f, -150.0f),
        XMFLOAT3(150.0f, -300.0f, -150.0f)
    };


    for (int i = 0; i < LanternCount; i++)
    {
        // 상하 움직임
        float floatOffsetY =
            sinf(rotation * 0.5f + i * 2.0f)
            * 10.0f;


        worldMatrix =
            XMMatrixScaling(
                50.0f,
                50.0f,
                50.0f
            )
            *
            XMMatrixRotationY(
                rotation * 0.5f + i
            )
            *
            XMMatrixTranslation(
                LanternPositions[i].x,
                LanternPositions[i].y + floatOffsetY,
                LanternPositions[i].z
            );


        m_Lantern->Render(
            m_D3D->GetDeviceContext()
        );


        result = m_TextureShader->Render(
            m_D3D->GetDeviceContext(),
            m_Lantern->GetIndexCount(),
            worldMatrix,
            viewMatrix,
            projectionMatrix,
            m_Lantern->GetTexture()
        );


        if (!result)
        {
            MessageBox(
                NULL,
                L"Lantern Render Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }
    }



    // =====================================================
    // Floating Island
    // =====================================================
    //
    // 멀리 배치된 12개의 Floating Island
    // 현재는 동일한 ModelClass를 여러 위치에 직접 렌더링
    // =====================================================

    const int farIslandCount = 12;


    XMFLOAT3 farIslandPositions[farIslandCount] =
    {
        XMFLOAT3(-2200.0f, -330.0f, -1400.0f),
        XMFLOAT3(-2100.0f, -280.0f,   800.0f),
        XMFLOAT3(-1400.0f, -360.0f,  2100.0f),
        XMFLOAT3(-600.0f,  -300.0f, -2300.0f),

        XMFLOAT3(800.0f,   -270.0f,  2200.0f),
        XMFLOAT3(1500.0f,  -340.0f, -2000.0f),
        XMFLOAT3(2200.0f,  -290.0f,  1100.0f),
        XMFLOAT3(2400.0f,  -320.0f,  -600.0f),

        XMFLOAT3(-2500.0f, -370.0f,   100.0f),
        XMFLOAT3(2500.0f,  -300.0f,   300.0f),
        XMFLOAT3(1100.0f,  -390.0f,  2400.0f),
        XMFLOAT3(-1100.0f, -310.0f, -2500.0f)
    };


    for (int i = 0; i < farIslandCount; i++)
    {
        worldMatrix =
            XMMatrixScaling(
                45.0f,
                45.0f,
                45.0f
            )
            *
            XMMatrixRotationY(
                i * 0.6f
            )
            *
            XMMatrixTranslation(
                farIslandPositions[i].x,
                farIslandPositions[i].y,
                farIslandPositions[i].z
            );


        m_FloatingIsland->Render(
            m_D3D->GetDeviceContext()
        );


        result = m_TextureShader->Render(
            m_D3D->GetDeviceContext(),
            m_FloatingIsland->GetIndexCount(),
            worldMatrix,
            viewMatrix,
            projectionMatrix,
            m_FloatingIsland->GetTexture()
        );


        if (!result)
        {
            MessageBox(
                NULL,
                L"Far FloatingIsland Render Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }
    }



    // =====================================================
    // Cloud
    // =====================================================
    //
    // 구름들은 원점을 중심으로 천천히 공전
    // =====================================================

    const int cloudCount = 8;


    XMFLOAT3 cloudBasePositions[cloudCount] =
    {
        XMFLOAT3(-1800.0f, -650.0f, -1200.0f),
        XMFLOAT3(-1200.0f, -580.0f,  1600.0f),
        XMFLOAT3(-500.0f,  -700.0f, -2100.0f),
        XMFLOAT3(700.0f,   -620.0f,  1900.0f),

        XMFLOAT3(1500.0f,  -680.0f, -1500.0f),
        XMFLOAT3(2100.0f,  -600.0f,   500.0f),
        XMFLOAT3(-2300.0f, -720.0f,   300.0f),
        XMFLOAT3(300.0f,   -640.0f,  2400.0f)
    };


    const float cloudOrbitSpeed = 0.01f;


    for (int i = 0; i < cloudCount; i++)
    {
        float baseX =
            cloudBasePositions[i].x;

        float baseZ =
            cloudBasePositions[i].z;


        // 원점 기준 거리 계산
        float cloudRadius =
            sqrtf(
                baseX * baseX
                + baseZ * baseZ
            );


        // 현재 위치의 기본 각도
        float baseAngle =
            atan2f(
                baseZ,
                baseX
            );


        // 시간에 따라 공전
        float orbitAngle =
            baseAngle
            + rotation * cloudOrbitSpeed;


        float x =
            cosf(orbitAngle) * cloudRadius;

        float z =
            sinf(orbitAngle) * cloudRadius;

        float y =
            cloudBasePositions[i].y;


        worldMatrix =
            XMMatrixScaling(
                100.0f,
                100.0f,
                100.0f
            )
            *
            XMMatrixTranslation(
                x,
                y,
                z
            );


        m_Cloud->Render(
            m_D3D->GetDeviceContext()
        );


        result = m_TextureShader->Render(
            m_D3D->GetDeviceContext(),
            m_Cloud->GetIndexCount(),
            worldMatrix,
            viewMatrix,
            projectionMatrix,
            m_Cloud->GetTexture()
        );


        if (!result)
        {
            MessageBox(
                NULL,
                L"Cloud Render Failed",
                L"Render Error",
                MB_OK
            );

            return false;
        }
    }



    // =====================================================
    // Billboard
    // =====================================================
    //
    // Billboard는 항상 카메라 방향을 바라봐야 하므로
    // 아직 GraphicsClass에서 직접 처리
    // =====================================================

    m_D3D->TurnOnAlphaBlending();


    // 현재 카메라의 Y축 회전값
    float cameraYaw =
        m_Camera->GetRotationY();


    worldMatrix =
        XMMatrixScaling(
            50.0f,
            50.0f,
            50.0f
        )
        *

        // 카메라를 바라보도록 회전
        XMMatrixRotationY(
            XMConvertToRadians(cameraYaw)
            + XM_PI
        )
        *

        XMMatrixTranslation(
            195.0f,
            30.0f,
            145.0f
        );


    m_Billboard->Render(
        m_D3D->GetDeviceContext()
    );


    result = m_TextureShader->Render(
        m_D3D->GetDeviceContext(),
        m_Billboard->GetIndexCount(),
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        m_Billboard->GetTexture()
    );


    if (!result)
    {
        MessageBox(
            NULL,
            L"Billboard Render Failed",
            L"Render Error",
            MB_OK
        );

        return false;
    }


    m_D3D->TurnOffAlphaBlending();



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

    result = m_InfoBitmap->Render(
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
    }



    // -----------------------------------------------------
    // Performance Text
    // -----------------------------------------------------

    worldMatrix = XMMatrixIdentity();


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
    }


    // 렌더링 상태 복구
    m_D3D->TurnOffAlphaBlending();
    m_D3D->TurnZBufferOn();


    // 한 프레임 렌더링 종료
    m_D3D->EndScene();


    return true;
}