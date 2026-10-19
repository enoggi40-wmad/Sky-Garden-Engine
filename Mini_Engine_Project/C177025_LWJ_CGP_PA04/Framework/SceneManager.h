#pragma once

#include <DirectXMath.h>

class Scene;
class TextureShaderClass;
class ResourceManager;
struct ID3D11DeviceContext;

enum class SceneType
{
	Title,
	Main,
	// 다른 Scene 타입 추가 가능
};

class SceneManager
{
public:
	SceneManager();
	~SceneManager();

	bool Initialize(ResourceManager* resourceManager);
		
	Scene* GetCurrentScene();
	void Update(float deltaTime);

	bool Render(
		ID3D11DeviceContext* deviceContext,
		TextureShaderClass* textureShader,
		DirectX::XMMATRIX projectionMatrix
	);

	void LoadScene(SceneType type);
	void Shutdown();
	

private:
	Scene* m_CurrentScene;
	ResourceManager* m_ResourceManager;

	SceneType m_CurrentSceneType;

	bool m_EnterPressed;
};