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

	// Guardian Lion GameObject
	m_GuardianLionObject = nullptr;
	m_GuardianLionRenderer = nullptr;

	// Scene
	m_Scene = nullptr;
}


GraphicsClass::GraphicsClass(const GraphicsClass& other)
{
	m_GuardianLionObject = nullptr;
	m_GuardianLionRenderer = nullptr;
	m_SakuraTreeObject = nullptr;
	m_SakuraTreeRenderer = nullptr;
}


GraphicsClass::~GraphicsClass()
{
}


bool GraphicsClass::Initialize(int screenWidth, int screenHeight, HWND hwnd)
{
	bool result;

	XMMATRIX baseViewMatrix;

	m_hwnd = hwnd;

	// Create the Direct3D object.
	m_D3D = new D3DClass;
	if(!m_D3D)
	{
		return false;
	}

	// Initialize the Direct3D object.
	result = m_D3D->Initialize(screenWidth, screenHeight, VSYNC_ENABLED, hwnd, FULL_SCREEN, SCREEN_DEPTH, SCREEN_NEAR);
	if(!result)
	{
		MessageBox(hwnd, L"Could not initialize Direct3D.", L"Error", MB_OK);
		return false;
	}

	// Create the camera object.
	m_Camera = new CameraClass;
	if(!m_Camera)
	{
		return false;
	}

	// Set the initial position of the camera.
	m_Camera->SetPosition(0.0f, 100.0f, 0.0f);
	m_Camera->SetRotation(5.0f, 0.0f, 0.0f);

	// Intialize the scene object.
	m_Scene = new Scene;

	if (!m_Scene)
	{
		return false;
	}
	
	// Initialize the model object.
	// Create the model object.

	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// Guardian Lion
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

	//// Guardian Lion GameObject 생성
	m_GuardianLionObject = new GameObject("GuardianLion");

	if (!m_GuardianLionObject)
	{
		return false;
	}

	// Guardian Lion의 Transform 설정
	// 기존 GraphicsClass::Render()에서 사용하던
	// 크기와 위치 값을 GameObject의 Transform으로 옮긴다.
	m_GuardianLionObject->transform.scale =
	{ 80.0f, 80.0f, 80.0f };

	m_GuardianLionObject->transform.position =
	{ 0.0f, 0.0f, 60.0f };


	// Guardian Lion에 Rotator Component 추가
	Rotator* lionRotator =
		new Rotator(m_GuardianLionObject, 1.0f);

	m_GuardianLionObject->AddComponent(lionRotator);


	// Guardian Lion에 ModelRenderer Component 생성
	// GraphicsClass의 멤버 변수에 저장한다.
	// 그래야 Render()에서도 같은 ModelRenderer를 사용할 수 있다.
	m_GuardianLionRenderer =
		new ModelRenderer(
			m_GuardianLionObject,
			m_GuardianLion
		);

	// GuardianLion GameObject의 Component 목록에도 등록
	m_GuardianLionObject->AddComponent(
		m_GuardianLionRenderer
	);

	// 마지막에 Scene에 등록
	m_Scene->AddGameObject(m_GuardianLionObject);

	// Guardian Lion 초기화 실패 시 에러 메시지 출력
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the guardian lion object.", L"Error", MB_OK);
		return false;
	}

	// Sakura Tree
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
		MessageBox(hwnd, L"Could not initialize the m_SakuraTree object.", L"Error", MB_OK);
		return false;
	}
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	
	////////////////////////////////////////////////////////////////////////////////////////////////////////
	// SakuraTree GameObject 생성
	m_SakuraTreeObject = new GameObject("SakuraTree");

	if (!m_SakuraTreeObject)
	{
		return false;
	}

	// SakuraTree의 Transform 설정
	m_SakuraTreeObject->transform.scale =
	{ 150.0f, 150.0f, 150.0f };

	m_SakuraTreeObject->transform.position =
	{ -160.0f, 50.0f, 180.0f };

	// SakuraTree에 ModelRenderer Component 붙이기
	m_SakuraTreeRenderer =
		new ModelRenderer(
			m_SakuraTreeObject,
			m_SakuraTree
		);

	m_SakuraTreeObject->AddComponent(
		m_SakuraTreeRenderer
	);

	m_Scene->AddGameObject(m_SakuraTreeObject);
	////////////////////////////////////////////////////////////////////////////////////////////////////////

	// Crystal Prism2
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
		MessageBox(hwnd, L"Could not initialize the crystal prism2 object.", L"Error", MB_OK);
		return false;
	}


	// Water Plane
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
		MessageBox(hwnd, L"Could not initialize the water plane object.", L"Error", MB_OK);
		return false;
	}

	// Floating Island
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
		MessageBox(hwnd, L"Could not initialize the floating island object.", L"Error", MB_OK);
		return false;
	}

	// Floating Alter
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

	// Lantern
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

	// Golden Spire
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
		MessageBox(hwnd, L"Could not initialize the m_GoldenSpire object.", L"Error", MB_OK);
		return false;
	}

	// Circle Stone
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
		MessageBox(hwnd, L"Could not initialize the m_CircleStone object.", L"Error", MB_OK);
		return false;
	}

	// Crystal Prism
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
		MessageBox(hwnd, L"Could not initialize the crystal prism object.", L"Error", MB_OK);
		return false;
	}

	
	// Cloud
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
		MessageBox(hwnd, L"Could not initialize the Cloud object.", L"Error", MB_OK);
		return false;
	}
	

	// Billboard
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
		MessageBox(hwnd, L"Could not initialize the billboard object.", L"Error", MB_OK);
		return false;
	}

	
	/////////////////////////////////////////////////////////////////////////////

	// Create the texture shader object.
	m_TextureShader = new TextureShaderClass;
	if (!m_TextureShader)
	{
		return false;
	}

	// Initialize the texture shader object.
	result = m_TextureShader->Initialize(m_D3D->GetDevice(), hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the texture shader object.", L"Error", MB_OK);
		return false;
	}


	// Create the bitmap object.
	m_Bitmap = new BitmapClass;
	if (!m_Bitmap)
	{
		return false;
	}
	
	// Initialize the bitmap object.
	result = m_Bitmap->Initialize(m_D3D->GetDevice(), screenWidth, screenHeight, L"./data/Title.dds", 800,600);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the bitmap object.", L"Error", MB_OK);
		return false;
	}

	// Info bitmap
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
		MessageBox(hwnd, L"Could not initialize the info bitmap object.", L"Error", MB_OK);
		return false;
	}

	// 1. 2D UI용 카메라 위치로 잠깐 이동
	m_Camera->SetPosition(0.0f, 0.0f, -1.0f);
	m_Camera->SetRotation(0.0f, 0.0f, 0.0f);

	// 2. 그 상태의 viewMatrix를 저장
	m_Camera->Render();
	m_Camera->GetViewMatrix(m_BaseViewMatrix);

	// 3. 다시 3D 메인 장면 시작 카메라 위치로 복구
	m_Camera->SetPosition(500.0f, 500.0f, 500.0f);
	m_Camera->SetRotation(45.0f, -135.0f, 0.0f);

	// Create the text object.
	m_Text = new TextClass;
	if (!m_Text)
	{
		return false;
	}

	// Initialize the text object.
	result = m_Text->Initialize(m_D3D->GetDevice(), m_D3D->GetDeviceContext(), hwnd, screenWidth, screenHeight, m_BaseViewMatrix);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the text object.", L"Error", MB_OK);
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

		m_GuardianLionObject = nullptr;
		m_GuardianLionRenderer = nullptr;
		m_SakuraTreeObject = nullptr;
		m_SakuraTreeRenderer = nullptr;
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

	if (m_GuardianLion)
	{
		m_GuardianLion->Shutdown();
		delete m_GuardianLion;
		m_GuardianLion = 0;
	}

	if (m_SakuraTree)
	{
		m_SakuraTree->Shutdown();
		delete m_SakuraTree;
		m_SakuraTree = 0;
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
	
	// Clear the buffers to begin the scene.
	m_D3D->BeginScene(0.60f, 0.82f, 1.0f, 1.0f);

	// Generate the view matrix based on the camera's position.
	m_Camera->Render();

	// Get the world, view, and projection matrices from the camera and d3d objects.
	m_Camera->GetViewMatrix(viewMatrix);
	m_D3D->GetWorldMatrix(worldMatrix);
	m_D3D->GetProjectionMatrix(projectionMatrix);

	m_D3D->GetOrthoMatrix(orthoMatrix);

	// Render title scene.
	if (m_ShowTitleScene)
	{
		m_D3D->TurnZBufferOff();

		worldMatrix = XMMatrixIdentity();

		result = m_Bitmap->Render(m_D3D->GetDeviceContext(), 0, 0);
		if (!result)
		{
			MessageBox(NULL, L"Title Bitmap Render Failed", L"Render Error", MB_OK);
			return false;
		}

		result = m_TextureShader->Render(
			m_D3D->GetDeviceContext(),
			m_Bitmap->GetIndexCount(),
			worldMatrix,
			m_BaseViewMatrix,
			orthoMatrix,
			m_Bitmap->GetTexture());

		if (!result)
		{
			MessageBox(NULL, L"Title Texture Render Failed", L"Render Error", MB_OK);
			return false;
		}

		m_D3D->TurnZBufferOn();

		m_D3D->EndScene();

		return true;
	}

	// Guardian Lion Render
	// 실제 모델 렌더링은 ModelRenderer Component가 담당한다.
	result = m_GuardianLionRenderer->Render(
		m_D3D->GetDeviceContext(),
		m_TextureShader,
		viewMatrix,
		projectionMatrix
	);

	if (!result)
	{
		MessageBox(
			NULL,
			L"GuardianLion Render Failed",
			L"Render Error",
			MB_OK
		);

		return false;
	}

	// SakuraTree Render
	result = m_SakuraTreeRenderer->Render(
		m_D3D->GetDeviceContext(),
		m_TextureShader,
		viewMatrix,
		projectionMatrix
	);

	if (!result)
	{
		MessageBox(
			NULL,
			L"SakuraTree Render Failed",
			L"Render Error",
			MB_OK
		);

		return false;
	}

	//Render WaterPlane
	worldMatrix =
		XMMatrixScaling(5.0f, 1.0f, 5.0f) *
		XMMatrixTranslation(0.0f, -100.0f, 0.0f);

	m_WaterPlane->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(
		m_D3D->GetDeviceContext(),
		m_WaterPlane->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_WaterPlane->GetTexture());

	if (!result)
	{
		MessageBox(NULL, L"WaterPlane Failed", L"Render Error", MB_OK);
		return false;
	}


	// Render duplicated floating islands around the start position.
	const int iscrystalCount = 10;
	const float radius = 700.0f;

	const float centerX = 0.0f;
	const float centerY = -300.0f;
	const float centerZ = 0.0f;

	const float orbitSpeed = 0.03f;   // 공전 속도
	const float selfSpeed = 1.0f;    // 자전 속도

	for (int i = 0; i < iscrystalCount; i++)
	{
		float baseAngle = (XM_2PI / iscrystalCount) * i;

		// 공전 각도
		float orbitAngle = baseAngle + rotation * orbitSpeed;

		float x = centerX + cosf(orbitAngle) * radius;
		float z = centerZ + sinf(orbitAngle) * radius;

		worldMatrix =
			XMMatrixScaling(50.0f, 50.0f, 50.0f) *

			// 기본 방향 + 자전
			XMMatrixRotationY(-orbitAngle + XM_PIDIV2 + rotation * selfSpeed) *

			XMMatrixTranslation(x, centerY, z);

		m_CrystalPrism->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(
			m_D3D->GetDeviceContext(),
			m_CrystalPrism->GetIndexCount(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_CrystalPrism->GetTexture());

		if (!result)
		{
			MessageBox(NULL, L"CrystalPrism Instance Failed", L"Render Error", MB_OK);
			return false;
		}
	}

	// Render 3 fixed floating yellow crystals.
	const int yellowCrystalCount = 4;

	// 맵에 고정된 노란 수정 위치
	XMFLOAT3 yellowCrystalPositions[yellowCrystalCount] =
	{
		XMFLOAT3(-550.0f, 120.0f,  550.0f),
		XMFLOAT3(550.0f, 120.0f,  550.0f),
		XMFLOAT3(-550.0f, 120.0f,  -550.0f),
		XMFLOAT3(550.0f, 120.0f,  -550.0f)
	};

	for (int i = 0; i < yellowCrystalCount; i++)
	{
		float floatOffsetY = sinf(rotation * 0.5f + i * 2.0f) * 10.0f;

		worldMatrix =
			XMMatrixScaling(50.0f, 50.0f, 50.0f) *
			XMMatrixRotationY(rotation * 0.5f + i) *
			XMMatrixTranslation(
				yellowCrystalPositions[i].x,
				yellowCrystalPositions[i].y + floatOffsetY,
				yellowCrystalPositions[i].z
			);

		m_CrystalPrism2->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(
			m_D3D->GetDeviceContext(),
			m_CrystalPrism2->GetIndexCount(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_CrystalPrism2->GetTexture());

		if (!result)
		{
			MessageBox(NULL, L"Yellow Crystal Render Failed", L"Render Error", MB_OK);
			return false;
		}
	}

	// 맵에 랜턴
	const int LanternCount = 4;

	XMFLOAT3 LanternPositions[LanternCount] =
	{
		XMFLOAT3(-150.0f, -300.0f,  150.0f),
		XMFLOAT3(150.0f, -300.0f,  150.0f),
		XMFLOAT3(-150.0f, -300.0f,  -150.0f),
		XMFLOAT3(150.0f, -300.0f,  -150.0f)
	};

	for (int i = 0; i < LanternCount; i++)
	{
		float floatOffsetY = sinf(rotation * 0.5f + i * 2.0f) * 10.0f;

		worldMatrix =
			XMMatrixScaling(50.0f, 50.0f, 50.0f) *
			XMMatrixRotationY(rotation * 0.5f + i) *
			XMMatrixTranslation(
				LanternPositions[i].x,
				LanternPositions[i].y + floatOffsetY,
				LanternPositions[i].z
			);

		m_Lantern->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(
			m_D3D->GetDeviceContext(),
			m_Lantern ->GetIndexCount(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_Lantern ->GetTexture());

		if (!result)
		{
			MessageBox(NULL, L"Lantern Render Failed", L"Render Error", MB_OK);
			return false;
		}
	}

	// Render far floating islands.
	const int farIslandCount = 12;

	XMFLOAT3 farIslandPositions[farIslandCount] =
	{
		XMFLOAT3(-2200.0f, -330.0f, -1400.0f),
		XMFLOAT3(-2100.0f, -280.0f,   800.0f),
		XMFLOAT3(-1400.0f, -360.0f,  2100.0f),
		XMFLOAT3(-600.0f, -300.0f, -2300.0f),

		XMFLOAT3(800.0f, -270.0f,  2200.0f),
		XMFLOAT3(1500.0f, -340.0f, -2000.0f),
		XMFLOAT3(2200.0f, -290.0f,  1100.0f),
		XMFLOAT3(2400.0f, -320.0f,  -600.0f),

		XMFLOAT3(-2500.0f, -370.0f,   100.0f),
		XMFLOAT3(2500.0f, -300.0f,   300.0f),
		XMFLOAT3(1100.0f, -390.0f,  2400.0f),
		XMFLOAT3(-1100.0f, -310.0f, -2500.0f)
	};

	for (int i = 0; i < farIslandCount; i++)
	{
		worldMatrix =
			XMMatrixScaling(45.0f, 45.0f, 45.0f) *
			XMMatrixRotationY(i * 0.6f) *
			XMMatrixTranslation(
				farIslandPositions[i].x,
				farIslandPositions[i].y,
				farIslandPositions[i].z
			);

		m_FloatingIsland->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(
			m_D3D->GetDeviceContext(),
			m_FloatingIsland->GetIndexCount(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_FloatingIsland->GetTexture());

		if (!result)
		{
			MessageBox(NULL, L"Far FloatingIsland Render Failed", L"Render Error", MB_OK);
			return false;
		}
	}

		

	

	// Render orbiting lower clouds.
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

	const float cloudOrbitSpeed = 0.01f;   // 공전 속도
	const float cloudSelfSpeed = 0.1f;     // 자전 속도

	for (int i = 0; i < cloudCount; i++)
	{
		float baseX = cloudBasePositions[i].x;
		float baseZ = cloudBasePositions[i].z;

		// 원점 기준 거리와 기본 각도 계산
		float cloudRadius = sqrtf(baseX * baseX + baseZ * baseZ);
		float baseAngle = atan2f(baseZ, baseX);

		// 공전 각도
		float orbitAngle = baseAngle + rotation * cloudOrbitSpeed;

		float x = cosf(orbitAngle) * cloudRadius;
		float z = sinf(orbitAngle) * cloudRadius;
		float y = cloudBasePositions[i].y;

		worldMatrix =
			XMMatrixScaling(100.0f, 100.0f, 100.0f) *
			//XMMatrixRotationX(XM_PIDIV2 + rotation * cloudSelfSpeed + i *0.0f) *
			XMMatrixTranslation(x, y, z);

		m_Cloud->Render(m_D3D->GetDeviceContext());

		result = m_TextureShader->Render(
			m_D3D->GetDeviceContext(),
			m_Cloud->GetIndexCount(),
			worldMatrix,
			viewMatrix,
			projectionMatrix,
			m_Cloud->GetTexture());

		if (!result)
		{
			MessageBox(NULL, L"Cloud Render Failed", L"Render Error", MB_OK);
			return false;
		}
	}

	// Render floating alter.
	worldMatrix =
		XMMatrixScaling(320.0f, 120.0f, 320.0f) *
		XMMatrixTranslation(0.0f, -80.0f, 0.0f);

	m_FloatingAlter->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(
		m_D3D->GetDeviceContext(),
		m_FloatingAlter->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_FloatingAlter->GetTexture());

	if (!result)
	{
		MessageBox(NULL, L"Floating_Alter Render Failed", L"Render Error", MB_OK);
		return false;
	}

	// Render golden spire
	worldMatrix =
		XMMatrixScaling(100.0f, 100.0f, 100.0f) *
		XMMatrixTranslation(0.0f, -300.0f, 0.0f);

	m_GoldenSpire->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(
		m_D3D->GetDeviceContext(),
		m_GoldenSpire->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_GoldenSpire->GetTexture());

	if (!result)
	{
		MessageBox(NULL, L"m_GoldenSpire Render Failed", L"Render Error", MB_OK);
		return false;
	}

	// Render CircleStone.
	worldMatrix =
		XMMatrixScaling(1000.0f, 120.0f, 1000.0f) *
		XMMatrixTranslation(0.0f, -500.0f, 0.0f);

	m_CircleStone->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(
		m_D3D->GetDeviceContext(),
		m_CircleStone->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_CircleStone->GetTexture());

	if (!result)
	{
		MessageBox(NULL, L"m_CircleStone Render Failed", L"Render Error", MB_OK);
		return false;
	}

	

	

	// Render billboard with alpha blending.
	m_D3D->TurnOnAlphaBlending();

	float cameraYaw = m_Camera->GetRotationY();

	worldMatrix =
		XMMatrixScaling(50.0f, 50.0f, 50.0f) *
		XMMatrixRotationY(XMConvertToRadians(cameraYaw) + XM_PI) *
		XMMatrixTranslation(195.0f, 30.0f, 145.0f);

	m_Billboard->Render(m_D3D->GetDeviceContext());

	result = m_TextureShader->Render(
		m_D3D->GetDeviceContext(),
		m_Billboard->GetIndexCount(),
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		m_Billboard->GetTexture());

	if (!result)
	{
		MessageBox(NULL, L"Billboard Render Failed", L"Render Error", MB_OK);
		return false;
	}

	m_D3D->TurnOffAlphaBlending();
	

	// Render 2D UI.
	m_D3D->TurnZBufferOff();
	m_D3D->TurnOnAlphaBlending();

	worldMatrix = XMMatrixIdentity();

	// Panel first.
	result = m_InfoBitmap->Render(m_D3D->GetDeviceContext(), 0, 15);
	if (!result)
	{
		MessageBox(NULL, L"Info Bitmap Render Failed", L"Render Error", MB_OK);
		return false;
	}

	result = m_TextureShader->Render(
		m_D3D->GetDeviceContext(),
		m_InfoBitmap->GetIndexCount(),
		worldMatrix,
		m_BaseViewMatrix,
		orthoMatrix,
		m_InfoBitmap->GetTexture());

	if (!result)
	{
		MessageBox(NULL, L"Info Bitmap Texture Render Failed", L"Render Error", MB_OK);
		return false;
	}

	// Text second.
	worldMatrix = XMMatrixIdentity();

	result = m_Text->Render(
		m_D3D->GetDeviceContext(),
		worldMatrix,
		orthoMatrix);

	if (!result)
	{
		MessageBox(NULL, L"Text Render Failed", L"Render Error", MB_OK);
		return false;
	}

	m_D3D->TurnOffAlphaBlending();
	m_D3D->TurnZBufferOn();

	m_D3D->EndScene();

	return true;
}