#include "MainScene.h"

#include "ModelRenderer.h"
#include "BillboardRenderer.h"
#include "ResourceManager.h"
#include "modelclass.h"
#include "Rotator.h"
#include "Orbiter.h"
#include "Floater.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "SkyBoxRenderer.h"
#include "FreeCameraController.h"
#include <Windows.h>
#include <cmath>
#include <string>


MainScene::MainScene()
{
}

MainScene::~MainScene()
{
}

void MainScene::Update(float deltaTime)
{
    // 기존 GameObject / Component들의 Update 실행
    Scene::Update(deltaTime);

    // 1번 키 : MainCamera
    if (GetAsyncKeyState('1') & 0x8000)
    {
        SetActiveCamera(m_MainCamera);
    }

    // 2번 키 : CutsceneCamera
    if (GetAsyncKeyState('2') & 0x8000)
    {
        SetActiveCamera(m_CutsceneCamera);
    }
}

void MainScene::Initialize(ResourceManager* resourceManager)
{
    // =====================================================
    // Main Camera
    // =====================================================
    GameObject* mainCamera = new GameObject("MainCamera");

    mainCamera->transform.SetLocalPosition({
        500.0f,
        500.0f,
        500.0f
        });

    mainCamera->transform.rotation =
    {
        45.0f,
        -135.0f,
        0.0f
    };

    m_MainCamera =
        mainCamera->AddComponent<CameraComponent>();
    mainCamera->AddComponent<FreeCameraController>();

    SetActiveCamera(m_MainCamera);

    AddGameObject(mainCamera);

    // =====================================================
    // CutScene Camera
    // =====================================================
    GameObject* cutsceneCamera =
        new GameObject("CutsceneCamera");

    cutsceneCamera->transform.SetLocalPosition({
        0.0f,
        -200.0f,
        -1500.0f
        });

    cutsceneCamera->transform.rotation =
    {
        0.0f,
        0.0f,
        0.0f
    };

    m_CutsceneCamera =
        cutsceneCamera->AddComponent<CameraComponent>();
    cutsceneCamera->AddComponent<FreeCameraController>();

    AddGameObject(cutsceneCamera);


    // =====================================================
    // SkyBox
    // =====================================================

    GameObject* sky =
        new GameObject("skymap");

    SkyBoxRenderer* skyRenderer =
        sky->AddComponent<SkyBoxRenderer>(
            resourceManager->GetCubeMap("skymap")
        );

    bool result =
        skyRenderer->Initialize(
            resourceManager->GetDevice()
        );

    if (!result)
    {
        MessageBox(
            NULL,
            L"SkyBoxRenderer Initialize Failed",
            L"SkyBox Error",
            MB_OK
        );

        delete sky;
        return;
    }


    // Scene에 등록
    AddGameObject(sky);



    // =====================================================
    // Magic Orb
    // =====================================================

    ModelClass* magicOrbModel =
        resourceManager->GetModel("MagicOrb");

    if (!magicOrbModel)
    {
        return;
    }

    GameObject* magicOrb =
        new GameObject("MagicOrb");

    magicOrb->transform.SetLocalPosition({
        0.0f,
        300.0f,
        0.0f
        });

    magicOrb->transform.scale =
    {
        10.0f,
        10.0f,
        10.0f
    };

    magicOrb->AddComponent<ModelRenderer>(
        magicOrbModel
    );

    AddGameObject(magicOrb);

    // =====================================================
    // Magic Billboard
    // =====================================================

    ModelClass* magicBillboardModel =
        resourceManager->GetModel("MagicBillboard");

    GameObject* magicBillboard =
        new GameObject("MagicBillboard");

    magicBillboard->transform.SetLocalPosition({
        0.0f,
        200.0f,
        0.0f
        });

    magicBillboard->transform.scale =
    {
        75.0f,
        75.0f,
        75.0f
    };

    magicBillboard->AddComponent<BillboardRenderer>(
        magicBillboardModel
    );

    AddGameObject(magicBillboard);


    // =====================================================
    // Guardian Lion
    // =====================================================

    ModelClass* guardianLionModel =
        resourceManager->GetModel("GuardianLion");

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

    guardianLion->transform.SetLocalPosition({
        0.0f,
        0.0f,
        60.0f
        });

    //guardianLion->AddComponent<Rotator>(1.0f);
    guardianLion->AddComponent<ModelRenderer>(guardianLionModel);


    // Scene에 등록
    AddGameObject(guardianLion);

    // =====================================================
    // Sakura Tree
    // =====================================================

    ModelClass* sakuraTreeModel =
        resourceManager->GetModel("SakuraTree");


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

    sakuraTree->transform.SetLocalPosition({
        -160.0f,
        50.0f,
        180.0f
        });


    sakuraTree->AddComponent<ModelRenderer>(sakuraTreeModel);

    AddGameObject(sakuraTree);


    // =====================================================
    // Floating Alter
    // =====================================================

    ModelClass* floatingAlterModel =
        resourceManager->GetModel("FloatingAlter");

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

    floatingAlter->transform.SetLocalPosition({
        0.0f,
        -80.0f,
        0.0f
        });

    floatingAlter->AddComponent<ModelRenderer>(floatingAlterModel);

    AddGameObject(floatingAlter);

    // =====================================================
    // Golden Spire
    // =====================================================
    ModelClass* goldenSpireModel =
        resourceManager->GetModel("GoldenSpire");

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

    goldenSpire->transform.SetLocalPosition({
        0.0f,
        -300.0f,
        0.0f
        });

    goldenSpire->AddComponent<ModelRenderer>(goldenSpireModel);

    AddGameObject(goldenSpire);

    // =====================================================
    // CircleStone
    // =====================================================
    ModelClass* circleStoneModel =
        resourceManager->GetModel("CircleStone");

    ModelClass* crystalPrismModel =
        resourceManager->GetModel("CrystalPrism");

    if (!circleStoneModel || !crystalPrismModel)
    {
        MessageBoxW(
            nullptr,
            L"CircleStone 또는 CrystalPrism 로드 실패",
            L"MainScene",
            MB_OK
        );

        return;
    }

    GameObject* circleStone =
        new GameObject("CircleStone");

    // 기존 위치와 크기
    circleStone->transform.SetLocalPosition({
        0.0f, -500.0f, 0.0f
        });

    circleStone->transform.scale =
    {
        1000.0f, 120.0f, 1000.0f
    };

    circleStone->AddComponent<ModelRenderer>(circleStoneModel);

    // 부모가 움직이면 아래의 수정들도 함께 움직인다.
    circleStone->AddComponent<Floater>(
        20.0f,
        0.5f,
        1.0f
    );

    AddGameObject(circleStone);




    // =====================================================
    // Crystal Prism (CircleStone 자식 12개)
    // =====================================================

    ModelClass* crystalModel =
        resourceManager->GetModel("CrystalPrism");

    if (!crystalModel)
    {
        return;
    }

    // 생성할 수정 개수
    const int crystalCount = 12;

    // 공전 중심과 반지름
    const float orbitRadius = 700.0f;

    // CircleStone의 스케일
    const float parentScaleX = 1000.0f;
    const float parentScaleY = 120.0f;
    const float parentScaleZ = 1000.0f;

    for (int i = 0; i < crystalCount; i++)
    {
        GameObject* crystal =
            new GameObject(
                "CrystalPrism_" + std::to_string(i + 1)
            );

        // 12개를 30도 간격으로 배치
        float startAngle =
            (XM_2PI / crystalCount) * i;

        // 부모 스케일을 고려한 로컬 위치
        float localX =
            cosf(startAngle) * orbitRadius / parentScaleX;

        float localZ =
            sinf(startAngle) * orbitRadius / parentScaleZ;

        // 기존 수정 높이 -300, CircleStone 높이 -500
        // 두 위치의 차이인 200을 부모 Y 스케일로 보정
        float localY = 200.0f / parentScaleY;

        // CircleStone을 부모로 설정
        // false: 로컬 변환 유지 방식이라고 가정
        crystal->SetParent(circleStone);

        crystal->transform.SetLocalPosition({
            localX,
            localY,
            localZ
            });

        // 부모 스케일을 역으로 보정
        crystal->transform.scale = {
            50.0f / parentScaleX,
            50.0f / parentScaleY,
            50.0f / parentScaleZ
        };

        crystal->AddComponent<ModelRenderer>(
            crystalModel
        );

        // CircleStone을 중심으로 공전
        crystal->AddComponent<Orbiter>(
            XMFLOAT3{
                0.0f,
                200.0f / parentScaleY,
                0.0f
            },
            orbitRadius / parentScaleX,
            0.03f,
            startAngle
        );

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
        resourceManager->GetModel("CrystalPrism2");

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
        crystal2->transform.SetLocalPosition({
            crystal2Positions[i].x,
            crystal2Positions[i].y,
            crystal2Positions[i].z
            });


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
        resourceManager->GetModel("Lantern");

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

        lantern->transform.SetLocalPosition({
            lanternPositions[i].x,
            lanternPositions[i].y,
            lanternPositions[i].z
            });


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
        resourceManager->GetModel("FloatingIsland");

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


        floatingIsland->transform.SetLocalPosition({
            floatingIslandPositions[i].x,
            floatingIslandPositions[i].y,
            floatingIslandPositions[i].z
            });


        // 기존 GraphicsClass에서 사용하던
        // XMMatrixRotationY(i * 0.6f)를 Transform으로 이동
        floatingIsland->transform.rotation.y =
            i * 0.6f;


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
        resourceManager->GetModel("Cloud");

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
        cloud->transform.SetLocalPosition({
            cloudPositions[i].x,
            cloudPositions[i].y,
            cloudPositions[i].z
            });

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