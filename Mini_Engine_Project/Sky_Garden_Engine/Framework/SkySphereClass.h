#pragma once

#include <d3d11.h>
#include <DirectXMath.h>

class SkySphereClass
{
private:
	struct VertexType
	{
		DirectX::XMFLOAT3 position;
		DirectX::XMFLOAT2 texture;
		DirectX::XMFLOAT3 normal;
	};

public:
	SkySphereClass();
	~SkySphereClass();

	bool Initialize(
		ID3D11Device* device,
		int latitudeLines,
		int longitudeLines
	);

	void Shutdown();

	void Render(
		ID3D11DeviceContext* deviceContext
	);

	int GetIndexCount() const;

private:
	bool InitializeBuffers(
		ID3D11Device* device,
		int latitudeLines,
		int longitudeLines
	);

	void ShutdownBuffers();

	void RenderBuffers(
		ID3D11DeviceContext* deviceContext
	);

private:
	ID3D11Buffer* m_VertexBuffer;
	ID3D11Buffer* m_IndexBuffer;

	int m_VertexCount;
	int m_IndexCount;
};