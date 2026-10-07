#pragma once

#include <string>
#include <unordered_map>

class ModelClass;
struct ID3D11Device;

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    // 모델 리소스를 로드하고 ResourceManager 내부에 저장
    // 같은 이름의 모델이 이미 존재하면 기존 모델 반환
    ModelClass* LoadModel(
        const std::string& name,
        ID3D11Device* device,
        const wchar_t* modelPath,
        const wchar_t* texturePath
    );

    // 이름을 이용해 이미 로드된 모델 검색
    ModelClass* GetModel(
        const std::string& name
    );

    // ResourceManager가 소유한 모든 모델 리소스 해제
    void Shutdown();

private:
    // 모델 이름을 Key로 사용
    // 예: "Cloud" -> ModelClass*
    std::unordered_map<std::string, ModelClass*> m_Models;
};