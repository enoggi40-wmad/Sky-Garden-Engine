#include "TitleScene.h"
#include "ResourceManager.h"
#include "GameObject.h"
#include "UIRenderer.h"
#include "BitmapClass.h"

TitleScene::TitleScene()
{
    m_Bitmap = nullptr;
}

TitleScene::~TitleScene()
{
}

void TitleScene::Initialize(
    ResourceManager* resourceManager)
{
    BitmapClass* titleBitmap =
        resourceManager->GetBitmap("Title");

    if (!titleBitmap)
    {
        MessageBox(
            NULL,
            L"Title Bitmap is null",
            L"Error",
            MB_OK
        );

        return;
    }

    GameObject* titleUI =
        new GameObject("TitleUI");

    titleUI->AddComponent<UIRenderer>(
        titleBitmap
    );

    AddGameObject(titleUI);


}