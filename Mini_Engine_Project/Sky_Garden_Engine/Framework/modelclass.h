////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _MODELCLASS_H_
#define _MODELCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>

using namespace DirectX;

///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "textureclass.h"
#include <vector>
#include "ModelData.h"

#include <fstream>
using namespace std;

////////////////////////////////////////////////////////////////////////////////
// Class name: ModelClass
////////////////////////////////////////////////////////////////////////////////
class ModelClass
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
	    XMFLOAT2 texture;
		XMFLOAT3 normal;

		// 노멀맵의 방향을 모델 공간으로 변환하기 위한 접선
		XMFLOAT3 tangent;
		XMFLOAT3 binormal;
	};


public:
	ModelClass();
	ModelClass(const ModelClass&);
	~ModelClass();

	bool Initialize(
		ID3D11Device*,
		const std::vector<ModelVertexData>&,
		const WCHAR*
	);

	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	// 흙 텍스처와 혼합 마스크 로드
	bool LoadBlendTextures(
		ID3D11Device* device,
		const WCHAR* secondTextureFilename,
		const WCHAR* alphaMapFilename
	);

	// 셰이더에 전달할 텍스처 반환
	ID3D11ShaderResourceView* GetSecondTexture();
	ID3D11ShaderResourceView* GetAlphaMap();

	// 노멀맵 텍스처 로드
	bool LoadNormalMap(
		ID3D11Device* device,
		const WCHAR* filename
	);

	// 노멀맵을 셰이더에 전달할 때 사용
	ID3D11ShaderResourceView* GetNormalMap();
	


private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, const WCHAR*);
	void ReleaseTexture();

	// 삼각형의 위치·UV를 이용해 각 정점의 접선 계산
	void CalculateTangents(VertexType* vertices);

	

private:
	ID3D11Buffer *m_vertexBuffer, *m_indexBuffer;
	int m_vertexCount, m_indexCount;
	TextureClass* m_Texture;
	TextureClass* m_SecondTexture = nullptr;
	TextureClass* m_AlphaMap = nullptr;
	TextureClass* m_NormalMap = nullptr;
	ModelVertexData* m_model;
};

#endif