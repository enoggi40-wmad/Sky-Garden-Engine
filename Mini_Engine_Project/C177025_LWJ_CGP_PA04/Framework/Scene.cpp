#include "Scene.h"

#include "GameObject.h"
#include "ModelRenderer.h"
#include "textureshaderclass.h"

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