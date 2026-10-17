#include "Scene.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "textureshaderclass.h"
#include "modelclass.h"
#include "ResourceManager.h"

#include "Rotator.h"
#include "Orbiter.h"
#include "Floater.h"

Scene::Scene()
{
}

Scene::~Scene()
{
    // Scene이 GameObject의 소유권을 가진다고 가정
    for (GameObject* gameObject : gameObjects)
    {
        delete gameObject;
    }

    gameObjects.clear();
}

void Scene::AddGameObject(GameObject* gameObject)
{
    gameObjects.push_back(gameObject);
}

void Scene::Update(float deltaTime)
{
    for (GameObject* gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}

bool Scene::Render(
    ID3D11DeviceContext* deviceContext,
    TextureShaderClass* textureShader,
    DirectX::XMMATRIX viewMatrix,
    DirectX::XMMATRIX projectionMatrix)
{
    // Scene 안에 있는 모든 GameObject를 하나씩 확인
    for (GameObject* gameObject : gameObjects)
    {
        // 현재 GameObject 안에 ModelRenderer가 있는지 찾는다.
        ModelRenderer* renderer =
            gameObject->GetComponent<ModelRenderer>();

        if (!renderer)
        {
            // ModelRenderer가 없으면 렌더링할 필요 없음
            continue;
		}

        bool result = renderer->Render(
            deviceContext,
            textureShader,
            viewMatrix,
            projectionMatrix
        );

        if (!result)
        {
            continue;
		}
    }

	return true;
}

void Scene::InitializeAssignment(ResourceManager* resourceManager)
{
    // 아직 아무 GameObject도 생성하지 않음
}

void Scene::Initialize(ResourceManager* resourceManager, ID3D11Device* device)
{
    // =====================================================
    // Guardian Lion
    // =====================================================

    ModelClass* guardianLionModel =
        resourceManager->LoadModel(
            "GuardianLion",
            device,
			L"./data/Guardian_Lion.obj",
			L"./data/Guardian_Lion.dds"
            );

    if (!guardianLionModel)
    {
        return;
    }

    // GameObject 생성
    GameObject* guardianLion =
        new GameObject("GuardianLion");

    // 위치 / 크기 설정
    guardianLion->transform.scale =
    {
        80.0f,
        80.0f,
        80.0f
    };

    guardianLion->transform.position =
    {
        0.0f,
        0.0f,
        60.0f
    };

    guardianLion->AddComponent<Rotator>(1.0f);
    guardianLion->AddComponent<ModelRenderer>(guardianLionModel);

    // Scene에 등록
    AddGameObject(guardianLion);
    
    // =====================================================
    // Sakura Tree
    // =====================================================

    ModelClass* sakuraTreeModel =
        resourceManager->LoadModel(
            "SakuraTree",
            device,
            L"./data/Sakura_Tree.obj",
            L"./data/Sakura_Tree.dds"
        );

    if (!sakuraTreeModel)
    {
        return;
    }

    GameObject* sakuraTree =
        new GameObject("SakuraTree");

    // Transform 설정
    sakuraTree->transform.scale =
    {
        150.0f,
        150.0f,
        150.0f
    };

    sakuraTree->transform.position =
    {
        -160.0f,
        50.0f,
        180.0f
    };

    // 렌더링 Component 추가
    /*ModelRenderer* sakuraRenderer =
        new ModelRenderer(
            sakuraTree,
            sakuraTreeModel
        );*/

    sakuraTree->AddComponent<ModelRenderer>(sakuraTreeModel);

    AddGameObject(sakuraTree);

    // =====================================================
    // Water Plane
    // =====================================================

    ModelClass* waterPlaneModel =
        resourceManager->LoadModel(
            "WaterPlane",
            device,
            L"./data/WaterPlane.obj",
            L"./data/WaterPlane.dds"
        );

    if (!waterPlaneModel)
    {
        return;
    }

    GameObject* waterPlane =
        new GameObject("WaterPlane");

    // 기존 GraphicsClass의 WorldMatrix 값을 Transform으로 이동
    waterPlane->transform.scale =
    {
        5.0f,
        1.0f,
        5.0f
    };

    waterPlane->transform.position =
    {
        0.0f,
        -100.0f,
        0.0f
    };

    // WaterPlane 모델을 렌더링하도록 Component 추가
    ModelRenderer* waterPlaneRenderer =
        new ModelRenderer(
            waterPlane,
            waterPlaneModel
        );

    waterPlane->AddComponent<ModelRenderer>(waterPlaneModel);

    AddGameObject(waterPlane);

    // =====================================================
    // Floating Alter
    // =====================================================

    ModelClass* floatingAlterModel =
        resourceManager->LoadModel(
            "FloatingAlter",
            device,
            L"./data/Floating_Alter.obj",
            L"./data/Floating_Alter.dds"
        );

    if (!floatingAlterModel)
    {
        return;
    }
    GameObject* floatingAlter =
            new GameObject("FloatingAlter");

        floatingAlter->transform.scale =
        {
            320.0f,
            120.0f,
            320.0f
        };

        floatingAlter->transform.position =
        {
            0.0f,
            -80.0f,
            0.0f
        };

        floatingAlter->AddComponent<ModelRenderer>(floatingAlterModel);

        AddGameObject(floatingAlter);

    // =====================================================
    // Golden Spire
    // =====================================================
    ModelClass* goldenSpireModel =
        resourceManager->LoadModel(
            "GoldenSpire",
            device,
            L"./data/Golden_Spire.obj",
            L"./data/Golden_Spire.dds"
        );

    if (!goldenSpireModel)
    {
        return;
    }

    GameObject* goldenSpire =
            new GameObject("GoldenSpire");

        goldenSpire->transform.scale =
        {
            100.0f,
            100.0f,
            100.0f
        };

        goldenSpire->transform.position =
        {
            0.0f,
            -300.0f,
            0.0f
        };

        goldenSpire->AddComponent<ModelRenderer>(goldenSpireModel);

        AddGameObject(goldenSpire);

    // =====================================================
    // Circle Stone
    // =====================================================
    ModelClass* circleStoneModel =
        resourceManager->LoadModel(
            "CircleStone",
            device,
            L"./data/Circle_Stone.obj",
            L"./data/Circle_Stone.dds"
        );

    if (!circleStoneModel)
    {
        return;
    }

    GameObject* circleStone =
            new GameObject("CircleStone");

        circleStone->transform.scale =
        {
            1000.0f,
            120.0f,
            1000.0f
        };

        circleStone->transform.position =
        {
            0.0f,
            -500.0f,
            0.0f
        };

        circleStone->AddComponent<ModelRenderer>(circleStoneModel);

        AddGameObject(circleStone);

    // =====================================================
    // Crystal Prism
    // =====================================================
    //
    // 하나의 ModelClass를 공유하는 10개의 GameObject 생성
    // 각 GameObject는 서로 다른 시작 각도에서 공전
    // =====================================================
    ModelClass* crystalPrismModel =
        resourceManager->LoadModel(
            "CrystalPrism",
            device,
            L"./data/Crystal_Prism.obj",
            L"./data/Crystal_Prism.dds"
        );

    if (!crystalPrismModel)
    {
        return;
    }

    const int crystalCount = 10;

    const float radius = 700.0f;

    // 공전 중심점
    DirectX::XMFLOAT3 orbitCenter =
    {
        0.0f,
        -300.0f,
        0.0f
    };


    for (int i = 0; i < crystalCount; i++)
    {
        GameObject* crystal =
            new GameObject("CrystalPrism");


        // 수정 크기 설정
        crystal->transform.scale =
        {
            50.0f,
            50.0f,
            50.0f
        };


        // 각 수정이 원 둘레에서 서로 다른 위치에서 시작하도록
        // 시작 각도를 균등하게 배치
		// DirectX::XM_2PI는 360도에 해당하는 라디안 값
        float startAngle =
            (DirectX::XM_2PI / crystalCount) * i;


        // 최초 위치 설정
        crystal->transform.position =
        {
            orbitCenter.x + cosf(startAngle) * radius,
            orbitCenter.y,
            orbitCenter.z + sinf(startAngle) * radius
        };
        
        crystal->AddComponent<ModelRenderer>(crystalPrismModel);

        crystal->AddComponent<Orbiter>(
            orbitCenter,
            radius,
            0.03f,
            startAngle
        );

        crystal->AddComponent<Rotator>(1.0f);

        AddGameObject(crystal);
    }

    // =====================================================
    // Crystal Prism2
    // =====================================================
    //
    // 4개의 CrystalPrism2 GameObject 생성
    // 각 오브젝트는 서로 다른 위치에서 상하 이동 + 자전
    // =====================================================
    ModelClass* crystalPrism2Model =
        resourceManager->LoadModel(
            "CrystalPrism2",
            device,
            L"./data/Crystal_Prism2.obj",
            L"./data/Crystal_Prism2.dds"
        );

    if (!crystalPrism2Model)
    {
        return;
    }
    const int crystal2Count = 4;

        // 각 CrystalPrism2의 기본 위치
        DirectX::XMFLOAT3 crystal2Positions[crystal2Count] =
        {
            DirectX::XMFLOAT3(-550.0f, 120.0f,  550.0f),
            DirectX::XMFLOAT3(550.0f, 120.0f,  550.0f),
            DirectX::XMFLOAT3(-550.0f, 120.0f, -550.0f),
            DirectX::XMFLOAT3(550.0f, 120.0f, -550.0f)
        };


        for (int i = 0; i < crystal2Count; i++)
        {
        
            GameObject* crystal2 =
                new GameObject("CrystalPrism2");


            // 크기 설정
            crystal2->transform.scale =
            {
                50.0f,
                50.0f,
                50.0f
            };


            // 각 CrystalPrism2의 기본 위치 설정
            crystal2->transform.position =
            {
                crystal2Positions[i].x,
                crystal2Positions[i].y,
                crystal2Positions[i].z
            };

                
            // 4개의 GameObject가 하나의 CrystalPrism2 ModelClass를 공유
            crystal2->AddComponent<ModelRenderer>(crystalPrism2Model);


        
            // 각 오브젝트의 상하 움직임 시작 타이밍을 다르게 설정
            float startPhase =
                i * 2.0f;

            crystal2->AddComponent<Floater>(
                10.0f,
                0.5f,
                startPhase
            );

        
            // Y축 자전
            crystal2->AddComponent<Rotator>(0.5f);


        
            AddGameObject(crystal2);
        }

    // =====================================================
    // Lantern
    // =====================================================
    //
    // 4개의 Lantern GameObject 생성
    // 각 Lantern은 상하 이동 + Y축 자전
    // =====================================================
        
    ModelClass* lanternModel =
        resourceManager->LoadModel(
            "Lantern",
            device,
            L"./data/Lantern.obj",
            L"./data/Lantern.dds"
        );

    if (!lanternModel)
    {
        return;
    }

    const int lanternCount = 4;

    DirectX::XMFLOAT3 lanternPositions[lanternCount] =
    {
        DirectX::XMFLOAT3(-150.0f, -300.0f,  150.0f),
        DirectX::XMFLOAT3(150.0f, -300.0f,  150.0f),
        DirectX::XMFLOAT3(-150.0f, -300.0f, -150.0f),
        DirectX::XMFLOAT3(150.0f, -300.0f, -150.0f)
    };


    for (int i = 0; i < lanternCount; i++)
    {
        // GameObject 생성
        GameObject* lantern =
            new GameObject("Lantern");


        // Transform 설정
        lantern->transform.scale =
        {
            50.0f,
            50.0f,
            50.0f
        };

        lantern->transform.position =
        {
            lanternPositions[i].x,
            lanternPositions[i].y,
            lanternPositions[i].z
        };


        // 모델 렌더링 Component
        lantern->AddComponent <ModelRenderer>(lanternModel);


        // 각 Lantern의 움직임 시작 시점을 다르게 설정
        float startPhase =
            i * 2.0f;


        // 상하 이동 Component
        lantern->AddComponent<Floater>(
            10.0f,
            0.5f,
            startPhase
        );


        // Y축 자전 Component
        lantern->AddComponent<Rotator>(0.5f);


        // Scene 등록
        AddGameObject(lantern);
    }

    // =====================================================
    // Floating Island
    // =====================================================
    //
    // 12개의 FloatingIsland GameObject 생성
    // 현재 움직임이 없으므로 Transform + ModelRenderer만 사용
    // =====================================================
    ModelClass* floatingIslandModel =
        resourceManager->LoadModel(
            "FloatingIsland",
            device,
            L"./data/Floating_Island.obj",
            L"./data/Floating_Island.dds"
        );

    if (!floatingIslandModel)
    {
        return;
    }

    const int floatingIslandCount = 12;

    DirectX::XMFLOAT3 floatingIslandPositions[floatingIslandCount] =
    {
        DirectX::XMFLOAT3(-2200.0f, -330.0f, -1400.0f),
        DirectX::XMFLOAT3(-2100.0f, -280.0f,   800.0f),
        DirectX::XMFLOAT3(-1400.0f, -360.0f,  2100.0f),
        DirectX::XMFLOAT3(-600.0f,  -300.0f, -2300.0f),

        DirectX::XMFLOAT3(800.0f,   -270.0f,  2200.0f),
        DirectX::XMFLOAT3(1500.0f,  -340.0f, -2000.0f),
        DirectX::XMFLOAT3(2200.0f,  -290.0f,  1100.0f),
        DirectX::XMFLOAT3(2400.0f,  -320.0f,  -600.0f),

        DirectX::XMFLOAT3(-2500.0f, -370.0f,   100.0f),
        DirectX::XMFLOAT3(2500.0f,  -300.0f,   300.0f),
        DirectX::XMFLOAT3(1100.0f,  -390.0f,  2400.0f),
        DirectX::XMFLOAT3(-1100.0f, -310.0f, -2500.0f)
    };


    for (int i = 0; i < floatingIslandCount; i++)
    {
        // GameObject 생성
        GameObject* floatingIsland =
            new GameObject("FloatingIsland");


        floatingIsland->transform.scale =
        {
            45.0f,
            45.0f,
            45.0f
        };


        floatingIsland->transform.position =
        {
            floatingIslandPositions[i].x,
            floatingIslandPositions[i].y,
            floatingIslandPositions[i].z
        };


        // 기존 GraphicsClass에서 사용하던
        // XMMatrixRotationY(i * 0.6f)를 Transform으로 이동
        floatingIsland->transform.rotation.y =
            i * 0.6f;


        ModelRenderer* floatingIslandRenderer =
            new ModelRenderer(
                floatingIsland,
                floatingIslandModel
            );

        floatingIsland->AddComponent<ModelRenderer>(floatingIslandModel);


        AddGameObject(floatingIsland);
    }

    // =====================================================
    // Cloud
    // =====================================================
    //
    // 8개의 Cloud GameObject 생성
    // 기존 위치를 기준으로 공전 반지름과 시작 각도를 계산
    // =====================================================
    ModelClass* cloudModel =
        resourceManager->LoadModel(
            "Cloud",
            device,
            L"./data/Cloud.obj",
            L"./data/Cloud.dds"
        );

    if (!cloudModel)
    {
        return;
    }    
        
    const int cloudCount = 8;

    DirectX::XMFLOAT3 cloudPositions[cloudCount] =
    {
        DirectX::XMFLOAT3(-1800.0f, -650.0f, -1200.0f),
        DirectX::XMFLOAT3(-1200.0f, -580.0f,  1600.0f),
        DirectX::XMFLOAT3(-500.0f,  -700.0f, -2100.0f),
        DirectX::XMFLOAT3(700.0f,   -620.0f,  1900.0f),

        DirectX::XMFLOAT3(1500.0f,  -680.0f, -1500.0f),
        DirectX::XMFLOAT3(2100.0f,  -600.0f,   500.0f),
        DirectX::XMFLOAT3(-2300.0f, -720.0f,   300.0f),
        DirectX::XMFLOAT3(300.0f,   -640.0f,  2400.0f)
    };


    // 공전 중심
    DirectX::XMFLOAT3 cloudOrbitCenter =
    {
        0.0f,
        0.0f,
        0.0f
    };


    for (int i = 0; i < cloudCount; i++)
    {
        GameObject* cloud =
            new GameObject("Cloud");


        cloud->transform.scale =
        {
            100.0f,
            100.0f,
            100.0f
        };


        // 기존 위치 설정
        cloud->transform.position =
        {
            cloudPositions[i].x,
            cloudPositions[i].y,
            cloudPositions[i].z
        };


        // 모델 렌더링 Component
        ModelRenderer* cloudRenderer =
            new ModelRenderer(
                cloud,
                cloudModel
            );

        cloud->AddComponent<ModelRenderer>(cloudModel);


        float baseX =
            cloudPositions[i].x;

        float baseZ =
            cloudPositions[i].z;


        // 원점으로부터 현재 Cloud까지의 거리
        float cloudRadius =
            sqrtf(
                baseX * baseX +
                baseZ * baseZ
            );


        // 현재 위치를 시작 각도로 변환
        float startAngle =
            atan2f(
                baseZ,
                baseX
            );

        cloud->AddComponent<Orbiter>(
            cloudOrbitCenter,
            cloudRadius,
            0.01f,
            startAngle
        );


        AddGameObject(cloud);
    }
}
