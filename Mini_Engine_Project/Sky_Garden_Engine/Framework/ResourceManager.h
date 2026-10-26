#pragma once

#include <string>
#include <unordered_map>
#include <d3d11.h>

class ModelClass;
struct ID3D11Device;
class BitmapClass;

class ResourceManager
{
public:
    ResourceManager();
    ~ResourceManager();

    void Initialize(ID3D11Device* device,
        int screenWidth,
        int screenHeight);

    // 모델 리소스를 로드하고 ResourceManager 내부에 저장
    // 같은 이름의 모델이 이미 존재하면 기존 모델 반환
    ModelClass* LoadModel(
        const std::string& name,
        const wchar_t* modelPath,
        const wchar_t* texturePath
    );
    
    

    // 이름을 이용해 이미 로드된 모델 검색
    ModelClass* GetModel(
        const std::string& name
    );

    // ResourceManager가 소유한 모든 모델 리소스 해제
    void Shutdown();

	// Bitmap 리소스를 로드하고 ResourceManager 내부에 저장
    BitmapClass* LoadBitmapResource(
        const std::string& name,
        const wchar_t* texturePath,
        int bitmapWidth,
        int bitmapHeight
    );

    BitmapClass* GetBitmap(
        const std::string& name
    );

    ID3D11ShaderResourceView* LoadCubeMap(
        const std::string& name,
        const wchar_t* texturePath
    );

    ID3D11ShaderResourceView* GetCubeMap(
        const std::string& name
    );

    ID3D11Device* GetDevice() const;
    

private:
    struct ModelResourceInfo
    {
        std::wstring modelPath;
        std::wstring texturePath;
    };

    ID3D11Device* m_Device;

    // 모델 이름을 Key로 사용
    // 예: "Cloud" -> ModelClass*
    std::unordered_map<std::string, ModelClass*> m_Models;


    std::unordered_map<std::string, ModelResourceInfo> m_ModelInfos;
    std::unordered_map<std::string, BitmapClass*> m_Bitmaps;

    std::unordered_map<std::string, ID3D11ShaderResourceView*> m_CubeMaps;
        
        
    

private:
   
    int m_ScreenWidth;
    int m_ScreenHeight;
};