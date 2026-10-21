#include "ResourceManager.h"
#include "modelclass.h"
#include "BitmapClass.h"


ResourceManager::ResourceManager()
{
    m_Device = nullptr;
}


ResourceManager::~ResourceManager()
{
}

BitmapClass* ResourceManager::GetBitmap(
    const std::string& name)
{
    auto it = m_Bitmaps.find(name);

    if (it != m_Bitmaps.end())
    {
        return it->second;
    }

    return nullptr;
}

BitmapClass* ResourceManager::LoadBitmapResource(
    const std::string& name,
    const wchar_t* texturePath,
    int bitmapWidth,
    int bitmapHeight)
{
    // 이미 로드된 Bitmap이 있으면 기존 것을 반환
    auto it = m_Bitmaps.find(name);

    if (it != m_Bitmaps.end())
    {
        return it->second;
    }


    // 새로운 Bitmap 생성
    BitmapClass* bitmap = new BitmapClass;

    if (!bitmap)
    {
        return nullptr;
    }


    // Bitmap 초기화
    bool result = bitmap->Initialize(
        m_Device,
        m_ScreenWidth,
        m_ScreenHeight,
        texturePath,
        bitmapWidth,
        bitmapHeight
    );

    if (!result)
    {
        delete bitmap;
        return nullptr;
    }


    // ResourceManager에 저장
    m_Bitmaps[name] = bitmap;

    return bitmap;
}

void ResourceManager::Initialize(ID3D11Device* device,
    int screenWidth,
    int screenHeight)
{
    m_Device = device;
    m_ScreenWidth = screenWidth;
    m_ScreenHeight = screenHeight;

    BitmapClass* title =
        LoadBitmapResource(
            "Title",
            L"./data/Title.dds",
            800,
            600
        );

    if (!title)
    {
        MessageBox(
            NULL,
            L"Title.dds Load Failed",
            L"Resource Error",
            MB_OK
        );
    }

    m_ModelInfos["MagicBillboard"] =
    {
        L"./data/Magic_Billboard.obj",
        L"./data/Magic_Billboard.dds"
    };

    m_ModelInfos["GuardianLion"] =
    {
        L"./data/Guardian_Lion.obj",
        L"./data/Guardian_Lion.dds"
    };

    m_ModelInfos["SakuraTree"] =
    {
       L"./data/Sakura_Tree.obj",
       L"./data/Sakura_Tree.dds"
    };

    m_ModelInfos["WaterPlane"] =
    {
       L"./data/WaterPlane.obj",
       L"./data/WaterPlane.dds"
    };

    m_ModelInfos["FloatingAlter"] =
    {
       L"./data/Floating_Alter.obj",
       L"./data/Floating_Alter.dds"
    };

    m_ModelInfos["GoldenSpire"] =
    {
       L"./data/Golden_Spire.obj",
       L"./data/Golden_Spire.dds"
    };

    m_ModelInfos["CircleStone"] =
    {
       L"./data/Circle_Stone.obj",
       L"./data/Circle_Stone.dds"
    };

    m_ModelInfos["CrystalPrism"] =
    {
       L"./data/Crystal_Prism.obj",
       L"./data/Crystal_Prism.dds"
    };

    m_ModelInfos["CrystalPrism2"] =
    {
       L"./data/Crystal_Prism2.obj",
       L"./data/Crystal_Prism2.dds"
    };

    m_ModelInfos["Lantern"] =
    {
       L"./data/Lantern.obj",
       L"./data/Lantern.dds"
    };

    m_ModelInfos["FloatingIsland"] =
    {
       L"./data/Floating_Island.obj",
       L"./data/Floating_Island.dds"
    };

    m_ModelInfos["Cloud"] =
    {
       L"./data/Cloud.obj",
       L"./data/Cloud.dds"
    };

    
}

ModelClass* ResourceManager::LoadModel(
    const std::string& name,
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
            m_Device,
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
    // 이미 로드된 모델이 있는지 확인
    auto modelIt =
        m_Models.find(name);

    if (modelIt != m_Models.end())
    {
        return modelIt->second;
    }

    // 아직 로드되지 않았다면
    // 모델의 경로 정보가 등록되어 있는지 확인
    auto infoIt = m_ModelInfos.find(name);

    if (infoIt == m_ModelInfos.end())
    {
        return nullptr;
    }

    // 등록된 경로 정보를 이용해 모델 로드
    return LoadModel(
        name,
        infoIt->second.modelPath.c_str(),
        infoIt->second.texturePath.c_str()
    );
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