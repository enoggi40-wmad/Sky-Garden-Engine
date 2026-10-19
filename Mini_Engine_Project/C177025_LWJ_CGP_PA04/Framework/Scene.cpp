#include "Scene.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "textureshaderclass.h"
#include "modelclass.h"
#include "ResourceManager.h"

#include "Rotator.h"
#include "Orbiter.h"
#include "Floater.h"
#include "CameraComponent.h"

Scene::Scene()
{
    m_MainCamera = nullptr;
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
    DirectX::XMMATRIX projectionMatrix)
{
	// Main Camera가 설정되어 있지 않으면 렌더링하지 않음
    if (!m_MainCamera)
    {
        return false;
    }

	// Main Camera의 Transform을 기준으로 View Matrix를 가져옴
    m_MainCamera->Render();

	XMMATRIX viewMatrix;
    m_MainCamera->GetViewMatrix(viewMatrix);

    // 기존 GameObject 랜더링
    for (GameObject* gameObject : gameObjects)
    {
        ModelRenderer* renderer =
            gameObject->GetComponent<ModelRenderer>();
        if (!renderer)
        {
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

void Scene::SetMainCamera(CameraComponent* camera)
{
    m_MainCamera = camera;
}
