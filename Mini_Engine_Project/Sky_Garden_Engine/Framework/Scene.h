#pragma once

#include <vector>
#include <DirectXMath.h>
#include <memory>
#include <vector>

#include "LightManager.h"

class Material;

class GameObject;
class ModelClass;
class ShaderManager;

class ResourceManager;
class RendererManager;

struct ID3D11DeviceContext;
struct ID3D11Device;

class CameraComponent;
class D3DClass;


class Scene
{
    friend class GameObject;

public:
    Scene();
    ~Scene();

    // Scene에 필요한 GameObject들을 생성하고 구성
    virtual void Initialize(ResourceManager* resourceManager);
           
    void InitializeAssignment(ResourceManager* resourceManager);

    // Scene 안에 GameObject를 추가
    void AddGameObject(GameObject* gameObject);

    // Scene 안의 모든 GameObject를 매 프레임 업데이트
    virtual void Update(float deltaTime);

    // Scene 안의 GameObject들을 순회하면서
    
    bool Render(
        D3DClass* d3d,
        ID3D11DeviceContext* deviceContext,
        ShaderManager* shaderManager,
        DirectX::XMMATRIX projectionMatrix,
        DirectX::XMMATRIX orthoMatrix
    );

    LightManager* GetLightManager() const
    {
        return m_LightManager;
    }

protected:
    CameraComponent* m_ActiveCamera;
	RendererManager* m_RendererManager;

public:
    void SetActiveCamera(CameraComponent* camera);

private:
    // 현재 Scene에 들어있는 GameObject 목록
    std::vector<GameObject*> gameObjects;

protected:
	// Scene이 소유한 Material 목록
    Material* CreateMaterial();

private:
    // Scene: Material을 소유하고 수명을 관리함
	// ModelRenderer: Material을 소유하지 않고 Scene이 소유한 Material을 참조함
    std::vector<std::unique_ptr<Material>> m_Materials;

    LightManager* m_LightManager = nullptr;

    void OnComponentAdded(Component* component);
    void OnComponentRemoved(Component* component);


	// Scene의 Ambient Color를 설정하는 멤버 변수
private:
    DirectX::XMFLOAT4 m_AmbientColor =
    {
        0.05f, 0.05f, 0.05f, 1.0f
    };

public:
    void SetAmbientColor(
        const DirectX::XMFLOAT4& color)
    {
        m_AmbientColor = color;
    }

    DirectX::XMFLOAT4 GetAmbientColor() const
    {
        return m_AmbientColor;
    }
};