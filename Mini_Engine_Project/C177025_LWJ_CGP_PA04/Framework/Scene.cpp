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

void Scene::Initialize(ResourceManager* resourceManager)
{
    
}
