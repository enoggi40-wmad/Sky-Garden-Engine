#pragma once

#include <vector>
#include <DirectXMath.h>

class GameObject;
class ModelClass;
class TextureShaderClass;

class ResourceManager;
class RendererManager;

struct ID3D11DeviceContext;
struct ID3D11Device;

class CameraComponent;
class D3DClass;


class Scene
{
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
        TextureShaderClass* textureShader,
        DirectX::XMMATRIX projectionMatrix
    );

protected:
    CameraComponent* m_ActiveCamera;
	RendererManager* m_RendererManager;

public:
    void SetActiveCamera(CameraComponent* camera);

private:
    // 현재 Scene에 들어있는 GameObject 목록
    std::vector<GameObject*> gameObjects;
};