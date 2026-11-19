#pragma once
////////////////////////////////////////////////////////////////////////////////
// Filename: lightshaderclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _PHONGSHADERCLASS_H_
#define _PHONGSHADERCLASS_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <directxmath.h>
#include <d3dcompiler.h>
#include "LightData.h"

#include <fstream>

using namespace std;
using namespace DirectX;


////////////////////////////////////////////////////////////////////////////////
// Class name: PhongShaderClass
////////////////////////////////////////////////////////////////////////////////
class PhongShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX world;
		XMMATRIX view;
		XMMATRIX projection;
	};

	struct CameraBufferType
	{
		XMFLOAT3 cameraPosition;
		float padding;
	};

	struct LightBufferType
	{
		XMFLOAT4 ambientColor;
		XMFLOAT4 specularColor;

		float specularPower;
		float padding[3];
	};

public:
	PhongShaderClass();
	PhongShaderClass(const PhongShaderClass&);
	~PhongShaderClass();

	bool Initialize(ID3D11Device*, HWND);
	void Shutdown();

	bool Render(
		ID3D11DeviceContext* deviceContext,
		int indexCount,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 cameraPosition,
		XMFLOAT4 ambientColor,
		XMFLOAT4 specularColor,
		float specularPower,
		const LightData& lightData
	);
	

private:
	bool InitializeShader(ID3D11Device*, HWND, const WCHAR*);
	void ShutdownShader();
	void OutputShaderErrorMessage(ID3D10Blob*, HWND, const WCHAR*);

	bool SetShaderParameters(
		ID3D11DeviceContext* deviceContext,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* texture,
		XMFLOAT3 cameraPosition,
		XMFLOAT4 ambientColor,
		XMFLOAT4 specularColor,
		float specularPower,
		const LightData& lightData
	);

	void RenderShader(ID3D11DeviceContext*, int);

private:
	ID3D11VertexShader* m_vertexShader;
	ID3D11PixelShader* m_pixelShader;
	ID3D11InputLayout* m_layout;
	ID3D11SamplerState* m_sampleState;
	ID3D11Buffer* m_matrixBuffer;
	ID3D11Buffer* m_lightBuffer;
	ID3D11Buffer* m_cameraBuffer;
	
	ID3D11Buffer* m_lightDataBuffer;
};

#endif