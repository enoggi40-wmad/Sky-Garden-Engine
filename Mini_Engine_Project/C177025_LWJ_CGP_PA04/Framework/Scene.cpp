#include "Scene.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "textureshaderclass.h"
#include "Rotator.h"
#include "modelclass.h"

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

        // ModelRenderer가 있는 GameObject만 렌더링
        if (renderer)
        {
            bool result = renderer->Render(
                deviceContext,
                textureShader,
                viewMatrix,
                projectionMatrix
            );

            // 하나라도 렌더링 실패하면 Scene 렌더링도 실패
            if (!result)
            {
                return false;
            }
        }
    }

    // 모든 GameObject 렌더링 성공
    return true;
}

void Scene::Initialize(
    ModelClass* guardianLionModel,
    ModelClass* sakuraTreeModel,
    ModelClass* waterPlaneModel,
    ModelClass* floatingAlterModel,
    ModelClass* goldenSpireModel,
    ModelClass* circleStoneModel)
{
    // =====================================================
    // Guardian Lion
    // =====================================================

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

    // Y축 회전 기능 추가
    Rotator* lionRotator =
        new Rotator(guardianLion, 1.0f);

    guardianLion->AddComponent(lionRotator);

    // 모델 렌더링 기능 추가
    ModelRenderer* lionRenderer =
        new ModelRenderer(
            guardianLion,
            guardianLionModel
        );

    guardianLion->AddComponent(lionRenderer);

    // Scene에 등록
    AddGameObject(guardianLion);



    // =====================================================
    // Sakura Tree
    // =====================================================

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
    ModelRenderer* sakuraRenderer =
        new ModelRenderer(
            sakuraTree,
            sakuraTreeModel
        );

    sakuraTree->AddComponent(sakuraRenderer);

    AddGameObject(sakuraTree);



    // =====================================================
    // Water Plane
    // =====================================================

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

    waterPlane->AddComponent(waterPlaneRenderer);

    AddGameObject(waterPlane);



    // =====================================================
    // Floating Alter
    // =====================================================

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

    ModelRenderer* floatingAlterRenderer =
        new ModelRenderer(
            floatingAlter,
            floatingAlterModel
        );

    floatingAlter->AddComponent(floatingAlterRenderer);

    AddGameObject(floatingAlter);



    // =====================================================
    // Golden Spire
    // =====================================================

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

    ModelRenderer* goldenSpireRenderer =
        new ModelRenderer(
            goldenSpire,
            goldenSpireModel
        );

    goldenSpire->AddComponent(goldenSpireRenderer);

    AddGameObject(goldenSpire);



    // =====================================================
    // Circle Stone
    // =====================================================

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

    ModelRenderer* circleStoneRenderer =
        new ModelRenderer(
            circleStone,
            circleStoneModel
        );

    circleStone->AddComponent(circleStoneRenderer);

    AddGameObject(circleStone);
}
