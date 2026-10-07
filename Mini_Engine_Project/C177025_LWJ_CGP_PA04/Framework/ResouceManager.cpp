#include "ResourceManager.h"
#include "modelclass.h"


ResourceManager::ResourceManager()
{
}


ResourceManager::~ResourceManager()
{
}


ModelClass* ResourceManager::LoadModel(
    const std::string& name,
    ID3D11Device* device,
    const wchar_t* modelPath,
    const wchar_t* texturePath)
{
    // 이미 같은 이름의 모델이 로드되어 있는지 확인
    auto it = m_Models.find(name);

    if (it != m_Models.end())
    {
        // 이미 존재하면 새로 만들지 않고 기존 모델 반환
        return it->second;
    }


    // 새로운 모델 리소스 생성
    ModelClass* model =
        new ModelClass;

    if (!model)
    {
        return nullptr;
    }


    // 모델 / 텍스처 초기화
    bool result =
        model->Initialize(
            device,
            modelPath,
            texturePath
        );

    if (!result)
    {
        delete model;
        return nullptr;
    }


    // ResourceManager 내부에 저장
    m_Models[name] = model;


    return model;
}


ModelClass* ResourceManager::GetModel(
    const std::string& name)
{
    auto it =
        m_Models.find(name);

    if (it != m_Models.end())
    {
        return it->second;
    }

    // 해당 이름의 모델이 없음
    return nullptr;
}


void ResourceManager::Shutdown()
{
    // ResourceManager가 소유한 모든 모델 리소스 정리
    for (auto& pair : m_Models)
    {
        ModelClass* model =
            pair.second;

        if (model)
        {
            model->Shutdown();
            delete model;
        }
    }

    m_Models.clear();
}