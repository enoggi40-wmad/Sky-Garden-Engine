#pragma once

#include <Windows.h>
#include <vector>
#include "ModelData.h"

class FbxLoader
{
public:
    FbxLoader();
    ~FbxLoader();

    bool Load(
        const WCHAR* filename,
        std::vector<ModelVertexData>& vertices
    );
};