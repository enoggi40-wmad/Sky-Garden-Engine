#include "FbxLoader.h"

#pragma comment(lib, "lib/assimp-vc142-mtd.lib")

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

#include <atlstr.h>

FbxLoader::FbxLoader()
{
}

FbxLoader::~FbxLoader()
{
}

bool FbxLoader::Load(
    const WCHAR* filename,
    std::vector<ModelVertexData>& vertices)
{
    Assimp::Importer importer;

    CString filePath = filename;

    std::string strPath = std::string(
        CT2CA(filePath.GetString())
    );

    const aiScene* scene = importer.ReadFile(
        strPath,
        aiProcess_Triangulate |
        aiProcess_ConvertToLeftHanded
    );

    if (!scene)
    {
        return false;
    }

    if (scene->mNumMeshes == 0)
    {
        return false;
    }

    const aiMesh* mesh = scene->mMeshes[0];

    if (!mesh)
    {
        return false;
    }

    vertices.clear();

    for (unsigned int i = 0; i < mesh->mNumFaces; i++)
    {
        aiFace face = mesh->mFaces[i];

        for (unsigned int j = 0; j < 3; j++)
        {
            unsigned int index = face.mIndices[j];

            ModelVertexData vertex;

            // Position
            vertex.x = mesh->mVertices[index].x;
            vertex.y = mesh->mVertices[index].y;
            vertex.z = mesh->mVertices[index].z;

            // UV
            if (mesh->HasTextureCoords(0))
            {
                vertex.tu =
                    mesh->mTextureCoords[0][index].x;

                vertex.tv =
                    mesh->mTextureCoords[0][index].y;
            }
            else
            {
                vertex.tu = 0.0f;
                vertex.tv = 0.0f;
            }

            // Normal
            if (mesh->HasNormals())
            {
                vertex.nx = mesh->mNormals[index].x;
                vertex.ny = mesh->mNormals[index].y;
                vertex.nz = mesh->mNormals[index].z;
            }
            else
            {
                vertex.nx = 0.0f;
                vertex.ny = 0.0f;
                vertex.nz = 0.0f;
            }

            vertices.push_back(vertex);
        }
    }

    return true;
}