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
    ModelClass* sakuraTreeModel)
{
    // =========================
    // GuardianLion
    // =========================

    GameObject* guardianLion =
        new GameObject("GuardianLion");

    // Transform 설정
    guardianLion->transform.scale =
    { 80.0f, 80.0f, 80.0f };

    guardianLion->transform.position =
    { 0.0f, 0.0f, 60.0f };


    // Rotator Component 생성 및 부착
    Rotator* lionRotator =
        new Rotator(guardianLion, 1.0f);

    guardianLion->AddComponent(lionRotator);


    // ModelRenderer Component 생성 및 부착
    ModelRenderer* lionRenderer =
        new ModelRenderer(
            guardianLion,
            guardianLionModel
        );

    guardianLion->AddComponent(lionRenderer);


    // Scene에 등록
    AddGameObject(guardianLion);



    // =========================
    // SakuraTree
    // =========================

    GameObject* sakuraTree =
        new GameObject("SakuraTree");

    // Transform 설정
    sakuraTree->transform.scale =
    { 150.0f, 150.0f, 150.0f };

    sakuraTree->transform.position =
    { -160.0f, 50.0f, 180.0f };


    // ModelRenderer Component 생성 및 부착
    ModelRenderer* sakuraRenderer =
        new ModelRenderer(
            sakuraTree,
            sakuraTreeModel
        );

    sakuraTree->AddComponent(sakuraRenderer);


    // Scene에 등록
    AddGameObject(sakuraTree);
}
