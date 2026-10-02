#pragma once

#include <vector>
#include <DirectXMath.h>

class GameObject;
class TextureShaderClass;
struct ID3D11DeviceContext;

class Scene
{
public:
    Scene();
    ~Scene();

    // Scene 안에 GameObject를 추가
    void AddGameObject(GameObject* gameObject);

    // Scene 안의 모든 GameObject를 매 프레임 업데이트
    void Update(float deltaTime);

    // Scene 안의 GameObject들을 순회하면서
    // ModelRenderer가 있는 오브젝트를 렌더링
    bool Render(
        ID3D11DeviceContext* deviceContext,
        TextureShaderClass* textureShader,
        DirectX::XMMATRIX viewMatrix,
        DirectX::XMMATRIX projectionMatrix
    );

private:
    // 현재 Scene에 들어있는 GameObject 목록
    std::vector<GameObject*> gameObjects;
};