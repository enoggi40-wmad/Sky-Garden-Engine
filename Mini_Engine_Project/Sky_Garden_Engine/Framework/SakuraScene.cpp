#include "SakuraScene.h"

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
#include "Material.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

SakuraScene::SakuraScene()
{
}

SakuraScene::~SakuraScene()
{
}

void SakuraScene::Initialize(ResourceManager* resourceManager)
{
	// =====================================================
    // 扁夯 炼疙
    // =====================================================
    SetAmbientColor({
    0.1f, 0.1f, 0.1f, 1.0f
        });

    GameObject* sunObject = new GameObject("SunLight");

    sunObject->transform.SetLocalEulerAngles({
        45.0f, -30.0f, 0.0f
        });

    DirectionalLight* sun =
        sunObject->AddComponent<DirectionalLight>();

    sun->SetDiffuseColor({
        1.0f, 0.94f, 0.85f, 1.0f
        });

    sun->SetIntensity(0.8f);

    AddGameObject(sunObject);

    // =====================================================
    // Main Camera
    // =====================================================
    GameObject* mainCamera = new GameObject("MainCamera");

    mainCamera->transform.SetWorldPosition({
        0.0f,
        0.0f,
        -500.0f
        });

    m_MainCamera =
        mainCamera->AddComponent<CameraComponent>();
    mainCamera->AddComponent<FreeCameraController>();

    SetActiveCamera(m_MainCamera);

    AddGameObject(mainCamera);
    
    // =====================================================
    // SkyBox
    // =====================================================

    GameObject* sky =
        new GameObject("skymap_Sunset");

    SkyBoxRenderer* skyRenderer =
        sky->AddComponent<SkyBoxRenderer>(
            resourceManager->GetCubeMap("skymap_Sunset")
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

	AddGameObject(sky);

    

    // =====================================================
    // Sakura Bridge
    // =====================================================

    ModelClass* sakurabridgeModel =
        resourceManager->GetModel("Sakura_Bridge");

    if (!sakurabridgeModel)
    {
        return;
    }

    // Material 积己
    Material* sakurabridgeMaterial = CreateMaterial();

    sakurabridgeMaterial->SetShaderType(ShaderType::Phong);

    sakurabridgeMaterial->SetDiffuseTexture(
        resourceManager->LoadTexture(
            "SakuraBridgeTexture",
            L"./data/Sakura/Sakura_Bridge.dds"
        )
    );

    // GameObject 积己
    GameObject* sakurabridge =
        new GameObject("Sakura_Bridge");

    // Transform
    sakurabridge->transform.SetWorldPosition({
        100.0f, 0.0f, 0.0f
        });

    sakurabridge->transform.SetLocalEulerAngles(
        { 90.0f, 0.0f, 0.0f }
        );

    sakurabridge->transform.SetWorldScale({
        100.0f, 100.0f, 100.0f
        });

    // Renderer
    sakurabridge->AddComponent<ModelRenderer>(
        sakurabridgeModel,
        sakurabridgeMaterial
    );

    // Scene 殿废
    AddGameObject(sakurabridge);


    // =====================================================
    // Sakura Terrace
    // =====================================================

    ModelClass* sakuraterraceModel =
        resourceManager->GetModel("Sakura_Terrace");
    if (!sakuraterraceModel)
    {
        return;
    }

    // Material 积己
    Material* sakuraterraceMaterial = CreateMaterial();

    sakuraterraceMaterial->SetShaderType(ShaderType::Phong);

    sakuraterraceMaterial->SetDiffuseTexture(
        resourceManager->LoadTexture(
            "SakuraTerraceTexture",
            L"./data/Sakura/Sakura_Terrace.dds"
        )
    );

    // GameObject 积己
    GameObject* sakuraterrace =
        new GameObject("Sakura_Terrace");

    // Transform
    sakuraterrace->transform.SetWorldPosition({
        0.0f, 0.0f, 0.0f
        });

    sakuraterrace->transform.SetLocalEulerAngles(
        { 90.0f, 0.0f, 0.0f }
    );

    sakuraterrace->transform.SetWorldScale({
        100.0f, 100.0f, 100.0f
        });

    // Renderer
    sakuraterrace->AddComponent<ModelRenderer>(
        sakuraterraceModel,
        sakuraterraceMaterial
    );

    // Scene 殿废
    AddGameObject(sakuraterrace);


    
}

void SakuraScene::Update(float deltaTime)
{
    Scene::Update(deltaTime);
}