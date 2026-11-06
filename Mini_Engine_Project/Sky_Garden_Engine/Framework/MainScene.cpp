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
#include "InputController.h"
#include <Windows.h>
#include <cmath>
#include <string>


MainScene::MainScene()
{
    m_MainCamera = nullptr;
    m_CutsceneCamera = nullptr;
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

    mainCamera->transform.SetWorldPosition({
        0.0f,
        -200.0f,
        -1600.0f
        });

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

    cutsceneCamera->transform.SetWorldPosition({
        500.0f,
        500.0f,
        500.0f
        });

    cutsceneCamera->transform.SetLocalEulerAngles({ 45.0f, -135.0f, 0.0f });

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
    // Magic Billboard
    // =====================================================

    ModelClass* magicBillboardModel =
        resourceManager->GetModel("MagicBillboard");

    GameObject* magicBillboard =
        new GameObject("MagicBillboard");

    magicBillboard->transform.SetWorldPosition({
        0.0f,
        200.0f,
        0.0f
        });

    magicBillboard->transform.SetWorldScale({
        75.0f,
        75.0f,
        75.0f
        });


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
    guardianLion->transform.SetWorldScale({
        80.0f,
        80.0f,
        80.0f
        });


    guardianLion->transform.SetWorldPosition({
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
    sakuraTree->transform.SetWorldScale({
        150.0f,
        150.0f,
        150.0f
        });


    sakuraTree->transform.SetWorldPosition({
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

    floatingAlter->transform.SetWorldScale({
        320.0f,
        120.0f,
        320.0f
        });

    floatingAlter->transform.SetWorldPosition({
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

    goldenSpire->transform.SetWorldScale({
        100.0f,
        100.0f,
        100.0f
        });


    goldenSpire->transform.SetWorldPosition({
        0.0f,
        -300.0f,
        0.0f
        });

    goldenSpire->AddComponent<ModelRenderer>(goldenSpireModel);

    AddGameObject(goldenSpire);

    // =====================================================
    // CircleStone Root
    // =====================================================

    GameObject* circleStoneRoot =
        new GameObject("CircleStoneRoot");

    circleStoneRoot->transform.SetWorldPosition({
        0.0f,
        0.0f,
        0.0f
        });
    circleStoneRoot->transform.SetWorldScale({
        1.0f,
        1.0f,
        1.0f
        });

    circleStoneRoot->AddComponent<InputController>(300.0f);

    AddGameObject(circleStoneRoot);

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
    circleStone->transform.SetWorldPosition({
        0.0f, -500.0f, 0.0f
        });

    circleStone->transform.SetWorldScale({
        1000.0f, 120.0f, 1000.0f
        });


    circleStone->AddComponent<ModelRenderer>(circleStoneModel);

    // 부모가 움직이면 아래의 수정들도 함께 움직인다.
    circleStone->AddComponent<Floater>(
        20.0f,
        0.5f,
        1.0f
    );

    circleStone->SetParent(circleStoneRoot);

    AddGameObject(circleStone);

    // =====================================================
    // Crystal Prism 중심 (빈 GameObject)
    // =====================================================
    // 공전 중심
    GameObject* orbitCenter =
        new GameObject("CrystalOrbitCenter");

    // CircleStone을 따라 이동
    orbitCenter->SetParent(circleStone, false);

    orbitCenter->transform.SetWorldPosition({
        0.0f, -300.0f, 0.0f
        });

    // =====================================================
    // Crystal Prism (CircleStone 자식 12개)
    // =====================================================

    AddGameObject(orbitCenter);

    ModelClass* crystalModel =
        resourceManager->GetModel("CrystalPrism");

    if (!crystalModel)
    {
        return;
    }

    const int crystalCount = 12;
    const float orbitRadius = 700.0f;

    for (int i = 0; i < crystalCount; i++)
    {
        GameObject* crystal = new GameObject(
            "CrystalPrism_" + std::to_string(i + 1)
        );

        float startAngle =
            (XM_2PI / crystalCount) * i;

        crystal->SetParent(circleStone, false);

        // 공전 중심
        crystal->transform.SetWorldPosition({
            0.0f, -300.0f, 0.0f
            });

        //crystal->transform.SetLocalEulerAngles({
        //45.0f,  // X축 회전
        //0.0f,
        //0.0f
        //    });

        // 크리스탈 크기
        crystal->transform.SetWorldScale({
            50.0f, 50.0f, 50.0f
            });

        crystal->AddComponent<ModelRenderer>(crystalModel);

        // 공전 Component 추가
        crystal->AddComponent<Orbiter>(
            &orbitCenter->transform,
            700.0f,
            0.3f,
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
        DirectX::XMFLOAT3(-400.0f, 120.0f,  400.0f),
        DirectX::XMFLOAT3(400.0f, 120.0f,  400.0f),
        DirectX::XMFLOAT3(-400.0f, 120.0f, -400.0f),
        DirectX::XMFLOAT3(400.0f, 120.0f, -400.0f)
    };


    for (int i = 0; i < crystal2Count; i++)
    {

        GameObject* crystal2 =
            new GameObject("CrystalPrism2");


        // 크기 설정
        crystal2->transform.SetWorldScale({
            50.0f,
            50.0f,
            50.0f
            });



        // 각 CrystalPrism2의 기본 위치 설정
        crystal2->transform.SetWorldPosition({
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
        lantern->transform.SetWorldScale({
            50.0f,
            50.0f,
            50.0f
            });


        lantern->transform.SetWorldPosition({
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
}