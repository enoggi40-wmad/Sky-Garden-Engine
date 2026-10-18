#include <Windows.h>
#include "SceneManager.h"

#include "Scene.h"
#include "ResourceManager.h"

#include "TitleScene.h"
#include "MainScene.h"




SceneManager::SceneManager()
{
    m_CurrentScene = nullptr;
    m_ResourceManager = nullptr;

    m_CurrentSceneType = SceneType::Title;

    m_EnterPressed = false;
}

SceneManager::~SceneManager()
{
}

void SceneManager::SetScene(Scene* scene)
{
    m_CurrentScene = scene;
}

Scene* SceneManager::GetCurrentScene()
{
    return m_CurrentScene;
}

void SceneManager::Update(float deltaTime)
{
    if (m_CurrentScene)
    {
        m_CurrentScene->Update(deltaTime);
    }

    bool enterDown =
        GetAsyncKeyState(VK_RETURN) & 0x8000;

    if (enterDown && !m_EnterPressed)
    {
        if (m_CurrentSceneType == SceneType::Title)
        {
            ChangeScene(SceneType::Main);
        }
    }

    m_EnterPressed = enterDown;
}

bool SceneManager::Render(
    ID3D11DeviceContext* deviceContext,
    TextureShaderClass* textureShader,
    DirectX::XMMATRIX viewMatrix,
    DirectX::XMMATRIX projectionMatrix)
{
    if (!m_CurrentScene)
    {
        return true;
    }

    return m_CurrentScene->Render(
        deviceContext,
        textureShader,
        viewMatrix,
        projectionMatrix
    );
}

bool SceneManager::Initialize(
    ResourceManager* resourceManager)
{
    m_ResourceManager = resourceManager;

    ChangeScene(SceneType::Title);

    return true;
}

void SceneManager::ChangeScene(SceneType type)
{
    if (m_CurrentScene)
    {
        delete m_CurrentScene;
        m_CurrentScene = nullptr;
    }

    switch (type)
    {
    case SceneType::Title:
        m_CurrentScene = new TitleScene;
        break;

    case SceneType::Main:
        m_CurrentScene = new MainScene;
        break;
    }

    m_CurrentSceneType = type;

    if (m_CurrentScene)
    {
        m_CurrentScene->Initialize(m_ResourceManager);
    }
}

void SceneManager::Shutdown()
{
    if (m_CurrentScene)
    {
        delete m_CurrentScene;
        m_CurrentScene = nullptr;
    }

    m_ResourceManager = nullptr;
}