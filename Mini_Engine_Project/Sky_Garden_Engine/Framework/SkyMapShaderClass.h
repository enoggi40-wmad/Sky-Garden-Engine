#pragma once

#include <d3d11.h>
#include <DirectXMath.h>
#include <d3dcompiler.h>

using namespace DirectX;

class SkyMapShaderClass
{
private:
	struct MatrixBufferType
	{
		XMMATRIX WVP;
		XMMATRIX World;
	};

public:
	SkyMapShaderClass();
	~SkyMapShaderClass();

	bool Initialize(ID3D11Device* device, HWND hwnd);
	void Shutdown();

	bool Render(
		ID3D11DeviceContext* deviceContext,
		int indexCount,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* cubeMapTexture
	);

private:
	bool InitializeShader(
		ID3D11Device* device,
		HWND hwnd,
		const WCHAR* shaderFilename
	);

	void ShutdownShader();

	bool SetShaderParameters(
		ID3D11DeviceContext* deviceContext,
		XMMATRIX worldMatrix,
		XMMATRIX viewMatrix,
		XMMATRIX projectionMatrix,
		ID3D11ShaderResourceView* cubeMapTexture
	);

	void RenderShader(
		ID3D11DeviceContext* deviceContext,
		int indexCount
	);

	void OutputShaderErrorMessage(
		ID3D10Blob* errorMessage,
		HWND hwnd,
		const WCHAR* shaderFilename
	);

private:
	ID3D11VertexShader* m_VertexShader;
	ID3D11PixelShader* m_PixelShader;

	ID3D11InputLayout* m_Layout;

	ID3D11Buffer* m_MatrixBuffer;

	ID3D11SamplerState* m_SampleState;
};