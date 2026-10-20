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
#include "Renderer.h"
#include "RendererManager.h"

Scene::Scene()
{
    m_ActiveCamera = nullptr;

    m_RendererManager =
        new RendererManager;
}

Scene::~Scene()
{
    // RendererManager는 Renderer를 소유하지 않음
    if (m_RendererManager)
    {
        delete m_RendererManager;
        m_RendererManager = nullptr;
    }

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

    Renderer* renderer =
        gameObject->GetComponent<Renderer>();

    if (renderer)
    {
        m_RendererManager->AddRenderer(renderer);
    }
}

void Scene::Update(float deltaTime)
{
    for (GameObject* gameObject : gameObjects)
    {
        gameObject->Update(deltaTime);
    }
}

bool Scene::Render(
    D3DClass* d3d,
    ID3D11DeviceContext* deviceContext,
    TextureShaderClass* textureShader,
    DirectX::XMMATRIX projectionMatrix)
{
	// Main Camera가 설정되어 있지 않으면 렌더링하지 않음
    if (!m_ActiveCamera)
    {
        return false;
    }

	// Main Camera의 Transform을 기준으로 View Matrix를 가져옴
    m_ActiveCamera->Render();

	XMMATRIX viewMatrix;
    m_ActiveCamera->GetViewMatrix(viewMatrix);

    XMFLOAT3 cameraPosition =
        m_ActiveCamera->GetPosition();

	// RendererManager를 통해 Scene 안의 모든 Renderer를 렌더링
    if (!m_RendererManager->Render(
        d3d,
        deviceContext,
        textureShader,
        viewMatrix,
        projectionMatrix,
        cameraPosition
    ))
    {
        return false;
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

void Scene::SetActiveCamera(CameraComponent* camera)
{
    // 기존 카메라 비활성화
    if (m_ActiveCamera)
    {
        m_ActiveCamera->SetActive(false);
    }

    // 새로운 카메라 지정
    m_ActiveCamera = camera;

    // 새로운 카메라 활성화
    if (m_ActiveCamera)
    {
        m_ActiveCamera->SetActive(true);
    }
}