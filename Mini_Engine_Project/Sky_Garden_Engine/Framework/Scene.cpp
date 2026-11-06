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
#include "Transform.h"
//
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
    ShaderManager* shaderManager,
    XMMATRIX projectionMatrix,
    XMMATRIX orthoMatrix)
{
    // 기본 카메라 정보
    XMMATRIX viewMatrix = XMMatrixIdentity();

    XMFLOAT3 cameraPosition = {
        0.0f, 0.0f, 0.0f
    };

    // 활성 카메라가 존재하는 경우
    if (m_ActiveCamera)
    {
        // 카메라의 Transform 가져오기
        const Transform& transform =
            m_ActiveCamera->GetTransform();

        // 카메라 월드 위치
        cameraPosition =
            transform.GetWorldPosition();

        // 카메라 월드 회전
        XMFLOAT4 cameraRotation =
            transform.GetWorldRotation();

        // DirectX 연산용 변환
        XMVECTOR position =
            XMLoadFloat3(&cameraPosition);

        XMVECTOR rotation =
            XMLoadFloat4(&cameraRotation);

        // 카메라 기본 전방(+Z)
        XMVECTOR forward = XMVector3Rotate(
            XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f),
            rotation
        );

        // 카메라 기본 위쪽(+Y)
        XMVECTOR up = XMVector3Rotate(
            XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f),
            rotation
        );

        // 최종 View Matrix 생성
        viewMatrix = XMMatrixLookToLH(
            position,
            forward,
            up
        );
    }

    // RendererManager에 카메라 정보 전달
    return m_RendererManager->Render(
        d3d,
        deviceContext,
        shaderManager,
        viewMatrix,
        projectionMatrix,
        orthoMatrix,
        cameraPosition
    );
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