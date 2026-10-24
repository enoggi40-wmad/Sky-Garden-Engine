#pragma once

#include <Windows.h>
#include <DirectXMath.h>
#include <vector>

#include "ModelData.h"


class ObjLoader
{
private:
    struct FaceType
    {
        int vIndex1, vIndex2, vIndex3;
        int tIndex1, tIndex2, tIndex3;
        int nIndex1, nIndex2, nIndex3;
    };


public:
    ObjLoader();
    ~ObjLoader();

    bool Load(
        const WCHAR* filename,
        std::vector<ModelVertexData>& vertices
    );


private:
    bool ReadFileCounts(
        const WCHAR* filename,
        int& vertexCount,
        int& textureCount,
        int& normalCount,
        int& faceCount
    );

    bool LoadDataStructures(
        const WCHAR* filename,
        int vertexCount,
        int textureCount,
        int normalCount,
        int faceCount,
        std::vector<ModelVertexData>& outputVertices
    );
};