#pragma once

#include "Scene.h"

class ResourceManager;
class BitmapClass;

class TitleScene : public Scene
{
public:
    TitleScene();
    ~TitleScene();

    void Initialize(ResourceManager* resourceManager) override;

private:
    BitmapClass* m_Bitmap;
};