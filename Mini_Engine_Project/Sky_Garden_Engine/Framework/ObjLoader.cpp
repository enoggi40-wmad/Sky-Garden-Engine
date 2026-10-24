#include "ObjLoader.h"

#include <fstream>

using namespace std;
using namespace DirectX;


ObjLoader::ObjLoader()
{
}


ObjLoader::~ObjLoader()
{
}


bool ObjLoader::Load(
    const WCHAR* filename,
    std::vector<ModelVertexData>& vertices)
{
    int vertexCount = 0;
    int textureCount = 0;
    int normalCount = 0;
    int faceCount = 0;


    // OBJ 내부 데이터 개수 확인
    bool result = ReadFileCounts(
        filename,
        vertexCount,
        textureCount,
        normalCount,
        faceCount
    );

    if (!result)
    {
        return false;
    }


    // 실제 OBJ 데이터 읽기
    result = LoadDataStructures(
        filename,
        vertexCount,
        textureCount,
        normalCount,
        faceCount,
        vertices
    );

    if (!result)
    {
        return false;
    }


    return true;
}

bool ObjLoader::ReadFileCounts(
    const WCHAR* filename,
    int& vertexCount,
    int& textureCount,
    int& normalCount,
    int& faceCount)
{
    ifstream fin;
    char input;


    vertexCount = 0;
    textureCount = 0;
    normalCount = 0;
    faceCount = 0;


    // OBJ 파일 열기
    fin.open(filename);

    if (fin.fail())
    {
        return false;
    }


    fin.get(input);

    while (!fin.eof())
    {
        // Vertex / Texture Coordinate / Normal
        if (input == 'v')
        {
            fin.get(input);

            if (input == ' ')
            {
                vertexCount++;
            }

            if (input == 't')
            {
                textureCount++;
            }

            if (input == 'n')
            {
                normalCount++;
            }
        }


        // Face
        if (input == 'f')
        {
            fin.get(input);

            if (input == ' ')
            {
                faceCount++;
            }
        }


        // 현재 줄의 나머지 부분 건너뛰기
        while (input != '\n')
        {
            fin.get(input);

            if (fin.eof())
            {
                break;
            }
        }


        fin.get(input);
    }


    fin.close();

    return true;
}

bool ObjLoader::LoadDataStructures(
    const WCHAR* filename,
    int vertexCount,
    int textureCount,
    int normalCount,
    int faceCount,
    std::vector<ModelVertexData>& outputVertices)
{
    vector<XMFLOAT3> vertices(vertexCount);
    vector<XMFLOAT2> texcoords(textureCount);
    vector<XMFLOAT3> normals(normalCount);
    vector<FaceType> faces(faceCount);

    ifstream fin;

    int vertexIndex = 0;
    int texcoordIndex = 0;
    int normalIndex = 0;
    int faceIndex = 0;

    char input;
    char input2;


    // OBJ 파일 열기
    fin.open(filename);

    if (fin.fail())
    {
        return false;
    }


    fin.get(input);

    while (!fin.eof())
    {
        if (input == 'v')
        {
            fin.get(input);


            // Vertex Position
            if (input == ' ')
            {
                fin
                    >> vertices[vertexIndex].x
                    >> vertices[vertexIndex].y
                    >> vertices[vertexIndex].z;


                // Right Hand -> Left Hand
                vertices[vertexIndex].z *= -1.0f;

                vertexIndex++;
            }


            // Texture Coordinate
            if (input == 't')
            {
                fin
                    >> texcoords[texcoordIndex].x
                    >> texcoords[texcoordIndex].y;


                // V 좌표 반전
                texcoords[texcoordIndex].y =
                    1.0f - texcoords[texcoordIndex].y;

                texcoordIndex++;
            }


            // Normal
            if (input == 'n')
            {
                fin
                    >> normals[normalIndex].x
                    >> normals[normalIndex].y
                    >> normals[normalIndex].z;


                // Right Hand -> Left Hand
                normals[normalIndex].z *= -1.0f;

                normalIndex++;
            }
        }


        // Face
        if (input == 'f')
        {
            fin.get(input);

            if (input == ' ')
            {
                // 역순으로 읽어서
                // Right Hand -> Left Hand 변환
                fin
                    >> faces[faceIndex].vIndex3
                    >> input2
                    >> faces[faceIndex].tIndex3
                    >> input2
                    >> faces[faceIndex].nIndex3

                    >> faces[faceIndex].vIndex2
                    >> input2
                    >> faces[faceIndex].tIndex2
                    >> input2
                    >> faces[faceIndex].nIndex2

                    >> faces[faceIndex].vIndex1
                    >> input2
                    >> faces[faceIndex].tIndex1
                    >> input2
                    >> faces[faceIndex].nIndex1;


                faceIndex++;
            }
        }


        // 현재 줄 나머지 건너뛰기
        while (input != '\n')
        {
            fin.get(input);

            if (fin.eof())
            {
                break;
            }
        }


        fin.get(input);
    }


    fin.close();


    // 삼각형 하나 = Vertex 3개
    outputVertices.clear();
    outputVertices.resize(faceIndex * 3);


    for (int i = 0; i < faceIndex; i++)
    {
        int vIndex;
        int tIndex;
        int nIndex;


        // ==============================
        // Vertex 1
        // ==============================

        vIndex = faces[i].vIndex1 - 1;
        tIndex = faces[i].tIndex1 - 1;
        nIndex = faces[i].nIndex1 - 1;


        outputVertices[i * 3].x =
            vertices[vIndex].x;

        outputVertices[i * 3].y =
            vertices[vIndex].y;

        outputVertices[i * 3].z =
            vertices[vIndex].z;


        outputVertices[i * 3].tu =
            texcoords[tIndex].x;

        outputVertices[i * 3].tv =
            texcoords[tIndex].y;


        outputVertices[i * 3].nx =
            normals[nIndex].x;

        outputVertices[i * 3].ny =
            normals[nIndex].y;

        outputVertices[i * 3].nz =
            normals[nIndex].z;


        // ==============================
        // Vertex 2
        // ==============================

        vIndex = faces[i].vIndex2 - 1;
        tIndex = faces[i].tIndex2 - 1;
        nIndex = faces[i].nIndex2 - 1;


        outputVertices[i * 3 + 1].x =
            vertices[vIndex].x;

        outputVertices[i * 3 + 1].y =
            vertices[vIndex].y;

        outputVertices[i * 3 + 1].z =
            vertices[vIndex].z;


        outputVertices[i * 3 + 1].tu =
            texcoords[tIndex].x;

        outputVertices[i * 3 + 1].tv =
            texcoords[tIndex].y;


        outputVertices[i * 3 + 1].nx =
            normals[nIndex].x;

        outputVertices[i * 3 + 1].ny =
            normals[nIndex].y;

        outputVertices[i * 3 + 1].nz =
            normals[nIndex].z;


        // ==============================
        // Vertex 3
        // ==============================

        vIndex = faces[i].vIndex3 - 1;
        tIndex = faces[i].tIndex3 - 1;
        nIndex = faces[i].nIndex3 - 1;


        outputVertices[i * 3 + 2].x =
            vertices[vIndex].x;

        outputVertices[i * 3 + 2].y =
            vertices[vIndex].y;

        outputVertices[i * 3 + 2].z =
            vertices[vIndex].z;


        outputVertices[i * 3 + 2].tu =
            texcoords[tIndex].x;

        outputVertices[i * 3 + 2].tv =
            texcoords[tIndex].y;


        outputVertices[i * 3 + 2].nx =
            normals[nIndex].x;

        outputVertices[i * 3 + 2].ny =
            normals[nIndex].y;

        outputVertices[i * 3 + 2].nz =
            normals[nIndex].z;
    }


    return true;
}