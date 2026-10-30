#include "TestScene.h"

#include "ResourceManager.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "FreeCameraController.h"
#include "SkyBoxRenderer.h"
#include "ModelRenderer.h"
#include "modelclass.h"
#include "Rotator.h"

#include <Windows.h>


TestScene::TestScene()
{
}

TestScene::~TestScene()
{
}


void TestScene::Initialize(ResourceManager* resourceManager)
{
    if (!resourceManager)
    {
        return;
    }

    // =====================================================
    // 1. 카메라
    // =====================================================
    GameObject* camera = new GameObject("TestCamera");

    camera->transform.position =
    {
        0.0f, 1000.0f, -700.0f
    };

    camera->transform.rotation =
    {
        60.0f, 0.0f, 0.0f
    };

    m_MainCamera =
        camera->AddComponent<CameraComponent>();

    camera->AddComponent<FreeCameraController>();

    SetActiveCamera(m_MainCamera);
    AddGameObject(camera);

    // =====================================================
    // 2. 스카이박스
    // =====================================================
    auto* cubeMap = resourceManager->GetCubeMap("skymap");

    if (!cubeMap)
    {
        MessageBoxW(
            nullptr,
            L"skymap 큐브맵이 로드되지 않았습니다.",
            L"TestScene",
            MB_OK
        );
        return;
    }

    GameObject* sky = new GameObject("TestSkyBox");

    SkyBoxRenderer* skyRenderer =
        sky->AddComponent<SkyBoxRenderer>(cubeMap);

    if (!skyRenderer->Initialize(resourceManager->GetDevice()))
    {
        delete sky;

        MessageBoxW(
            nullptr,
            L"스카이박스 초기화에 실패했습니다.",
            L"TestScene",
            MB_OK
        );
        return;
    }

    AddGameObject(sky);

    // =====================================================
    // 3. 멀티 텍스처 지면
    // =====================================================
    ModelClass* groundModel =
        resourceManager->GetModel("Ground");

    if (!groundModel)
    {
        MessageBoxW(
            nullptr,
            L"Ground 모델 로드에 실패했습니다.",
            L"TestScene",
            MB_OK
        );
        return;
    }

    // 기본 풀 텍스처에 흙과 S자 마스크 추가
    if (!groundModel->LoadBlendTextures(
        resourceManager->GetDevice(),
        L"./data/dirt.dds",
        L"./data/S.dds"))
    {
        MessageBoxW(
            nullptr,
            L"흙 텍스처 또는 알파맵 로드에 실패했습니다.",
            L"TestScene",
            MB_OK
        );
        return;
    }

    GameObject* ground = new GameObject("Ground");

    ground->transform.position =
    {
        0.0f, 0.0f, 0.0f
    };

    ground->transform.scale =
    {
        1000.0f, 1.0f, 1000.0f
    };

    ModelRenderer* groundRenderer =
        ground->AddComponent<ModelRenderer>(groundModel);

    groundRenderer->SetShaderType(ShaderType::AlphaMap);

    AddGameObject(ground);

    // =====================================================
    // 4. 세 구가 공유할 모델과 텍스처
    // =====================================================
    ModelClass* sphereModel =
        resourceManager->LoadModel(
            "StoneSphere",
            L"./data/Sphere.obj",
            L"./data/Stone.dds"
        );

    if (!sphereModel)
    {
        MessageBoxW(
            nullptr,
            L"Sphere.obj 또는 Stone.dds 로드에 실패했습니다.",
            L"TestScene",
            MB_OK
        );
        return;
    }

    if (!sphereModel->LoadNormalMap(
        resourceManager->GetDevice(),
        L"./data/Stone_Normal.dds"))
    {
        MessageBoxW(
            nullptr,
            L"Stone_Normal.dds 로드에 실패했습니다.",
            L"TestScene",
            MB_OK
        );
        return;
    }

    const float rotateSpeed = 1.0f;

    // =====================================================
    // 5. 왼쪽 구: 기본 텍스처
    // =====================================================
    GameObject* textureSphere =
        new GameObject("TextureSphere");

    textureSphere->transform.position =
    {
        -300.0f, 100.0f, 0.0f
    };

    textureSphere->transform.scale =
    {
        50.0f, 50.0f, 50.0f
    };

    ModelRenderer* textureRenderer =
        textureSphere->AddComponent<ModelRenderer>(sphereModel);

    textureRenderer->SetShaderType(ShaderType::Texture);

    textureSphere->AddComponent<Rotator>(rotateSpeed);

    AddGameObject(textureSphere);

    // =====================================================
    // 6. 가운데 구: Phong 조명
    // =====================================================
    GameObject* phongSphere =
        new GameObject("PhongSphere");

    phongSphere->transform.position =
    {
        0.0f, 100.0f, 0.0f
    };

    phongSphere->transform.scale =
    {
        50.0f, 50.0f, 50.0f
    };

    ModelRenderer* phongRenderer =
        phongSphere->AddComponent<ModelRenderer>(sphereModel);

    phongRenderer->SetShaderType(ShaderType::PhongLight);

    phongSphere->AddComponent<Rotator>(rotateSpeed);

    AddGameObject(phongSphere);

    // =====================================================
    // 7. 오른쪽 구: 노멀맵 + Phong 조명
    // =====================================================
    GameObject* bumpSphere =
        new GameObject("BumpMapSphere");

    bumpSphere->transform.position =
    {
        300.0f, 100.0f, 0.0f
    };

    bumpSphere->transform.scale =
    {
        50.0f, 50.0f, 50.0f
    };

    ModelRenderer* bumpRenderer =
        bumpSphere->AddComponent<ModelRenderer>(sphereModel);

    bumpRenderer->SetShaderType(ShaderType::BumpMap);

    bumpSphere->AddComponent<Rotator>(rotateSpeed);

    AddGameObject(bumpSphere);
}


void TestScene::Update(float deltaTime)
{
    // 카메라 조작과 각 구의 Rotator 업데이트
    Scene::Update(deltaTime);
}