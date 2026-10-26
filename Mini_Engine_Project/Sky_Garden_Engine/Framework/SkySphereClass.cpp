#include "SkySphereClass.h"

#include <vector>

using namespace DirectX;


SkySphereClass::SkySphereClass()
{
	m_VertexBuffer = 0;
	m_IndexBuffer = 0;

	m_VertexCount = 0;
	m_IndexCount = 0;
}


SkySphereClass::~SkySphereClass()
{
}


bool SkySphereClass::Initialize(
	ID3D11Device* device,
	int latitudeLines,
	int longitudeLines
)
{
	return InitializeBuffers(
		device,
		latitudeLines,
		longitudeLines
	);
}


void SkySphereClass::Shutdown()
{
	ShutdownBuffers();

	return;
}


void SkySphereClass::Render(
	ID3D11DeviceContext* deviceContext
)
{
	RenderBuffers(deviceContext);

	return;
}


int SkySphereClass::GetIndexCount() const
{
	return m_IndexCount;
}


bool SkySphereClass::InitializeBuffers(
	ID3D11Device* device,
	int latitudeLines,
	int longitudeLines
)
{
	int vertexCount =
		((latitudeLines - 2) * longitudeLines) + 2;

	int faceCount =
		((latitudeLines - 3) * longitudeLines * 2)
		+ (longitudeLines * 2);

	m_VertexCount = vertexCount;
	m_IndexCount = faceCount * 3;


	std::vector<VertexType> vertices(m_VertexCount);
	std::vector<unsigned long> indices(m_IndexCount);


	// Sphere의 시작점
	vertices[0].position =
		XMFLOAT3(0.0f, 0.0f, 1.0f);


	float sphereYaw = 0.0f;
	float spherePitch = 0.0f;


	XMVECTOR currentVertexPosition;

	XMMATRIX rotationX;
	XMMATRIX rotationZ;


	int vertexIndex = 1;


	// 중간 부분의 Sphere Vertex 생성
	for (int i = 0; i < latitudeLines - 2; ++i)
	{
		spherePitch =
			(float)(i + 1) *
			(XM_PI / (float)(latitudeLines - 1));


		rotationX =
			XMMatrixRotationX(spherePitch);


		for (int j = 0; j < longitudeLines; ++j)
		{
			sphereYaw =
				(float)j *
				(XM_2PI / (float)longitudeLines);


			rotationZ =
				XMMatrixRotationZ(sphereYaw);


			currentVertexPosition =
				XMVector3TransformNormal(
					XMVectorSet(
						0.0f,
						0.0f,
						1.0f,
						0.0f
					),
					rotationX * rotationZ
				);


			currentVertexPosition =
				XMVector3Normalize(
					currentVertexPosition
				);


			vertices[vertexIndex].position =
				XMFLOAT3(
					XMVectorGetX(currentVertexPosition),
					XMVectorGetY(currentVertexPosition),
					XMVectorGetZ(currentVertexPosition)
				);


			// 실제 SkyMap Shader에서는 사용하지 않음
			vertices[vertexIndex].texture =
				XMFLOAT2(0.0f, 0.0f);

			vertices[vertexIndex].normal =
				XMFLOAT3(0.0f, 0.0f, 0.0f);


			vertexIndex++;
		}
	}


	// Sphere 반대쪽 끝점
	vertices[m_VertexCount - 1].position =
		XMFLOAT3(0.0f, 0.0f, -1.0f);

	vertices[0].texture =
		XMFLOAT2(0.0f, 0.0f);

	vertices[0].normal =
		XMFLOAT3(0.0f, 0.0f, 0.0f);

	vertices[m_VertexCount - 1].texture =
		XMFLOAT2(0.0f, 0.0f);

	vertices[m_VertexCount - 1].normal =
		XMFLOAT3(0.0f, 0.0f, 0.0f);


	int index = 0;


	// 첫 번째 끝점과 첫 번째 Ring 연결
	for (int i = 0; i < longitudeLines - 1; ++i)
	{
		indices[index] = 0;
		indices[index + 1] = i + 1;
		indices[index + 2] = i + 2;

		index += 3;
	}


	indices[index] = 0;
	indices[index + 1] = longitudeLines;
	indices[index + 2] = 1;

	index += 3;


	// Sphere 중간 Ring 연결
	for (int i = 0; i < latitudeLines - 3; ++i)
	{
		for (int j = 0; j < longitudeLines - 1; ++j)
		{
			indices[index] =
				i * longitudeLines + j + 1;

			indices[index + 1] =
				i * longitudeLines + j + 2;

			indices[index + 2] =
				(i + 1) * longitudeLines + j + 1;


			indices[index + 3] =
				(i + 1) * longitudeLines + j + 1;

			indices[index + 4] =
				i * longitudeLines + j + 2;

			indices[index + 5] =
				(i + 1) * longitudeLines + j + 2;


			index += 6;
		}


		// Ring의 마지막과 처음 연결
		indices[index] =
			i * longitudeLines + longitudeLines;

		indices[index + 1] =
			i * longitudeLines + 1;

		indices[index + 2] =
			(i + 1) * longitudeLines + longitudeLines;


		indices[index + 3] =
			(i + 1) * longitudeLines + longitudeLines;

		indices[index + 4] =
			i * longitudeLines + 1;

		indices[index + 5] =
			(i + 1) * longitudeLines + 1;


		index += 6;
	}


	// 마지막 Ring과 마지막 끝점 연결
	for (int i = 0; i < longitudeLines - 1; ++i)
	{
		indices[index] =
			m_VertexCount - 1;

		indices[index + 1] =
			(m_VertexCount - 1) - (i + 1);

		indices[index + 2] =
			(m_VertexCount - 1) - (i + 2);

		index += 3;
	}


	indices[index] =
		m_VertexCount - 1;

	indices[index + 1] =
		(m_VertexCount - 1) - longitudeLines;

	indices[index + 2] =
		m_VertexCount - 2;


	// Vertex Buffer 생성
	D3D11_BUFFER_DESC vertexBufferDesc;

	ZeroMemory(
		&vertexBufferDesc,
		sizeof(vertexBufferDesc)
	);

	vertexBufferDesc.Usage =
		D3D11_USAGE_DEFAULT;

	vertexBufferDesc.ByteWidth =
		sizeof(VertexType) * m_VertexCount;

	vertexBufferDesc.BindFlags =
		D3D11_BIND_VERTEX_BUFFER;


	D3D11_SUBRESOURCE_DATA vertexData;

	ZeroMemory(
		&vertexData,
		sizeof(vertexData)
	);

	vertexData.pSysMem =
		vertices.data();


	HRESULT result =
		device->CreateBuffer(
			&vertexBufferDesc,
			&vertexData,
			&m_VertexBuffer
		);

	if (FAILED(result))
	{
		return false;
	}


	// Index Buffer 생성
	D3D11_BUFFER_DESC indexBufferDesc;

	ZeroMemory(
		&indexBufferDesc,
		sizeof(indexBufferDesc)
	);

	indexBufferDesc.Usage =
		D3D11_USAGE_DEFAULT;

	indexBufferDesc.ByteWidth =
		sizeof(unsigned long) * m_IndexCount;

	indexBufferDesc.BindFlags =
		D3D11_BIND_INDEX_BUFFER;


	D3D11_SUBRESOURCE_DATA indexData;

	ZeroMemory(
		&indexData,
		sizeof(indexData)
	);

	indexData.pSysMem =
		indices.data();


	result =
		device->CreateBuffer(
			&indexBufferDesc,
			&indexData,
			&m_IndexBuffer
		);

	if (FAILED(result))
	{
		return false;
	}


	return true;
}


void SkySphereClass::ShutdownBuffers()
{
	if (m_IndexBuffer)
	{
		m_IndexBuffer->Release();
		m_IndexBuffer = 0;
	}

	if (m_VertexBuffer)
	{
		m_VertexBuffer->Release();
		m_VertexBuffer = 0;
	}

	return;
}


void SkySphereClass::RenderBuffers(
	ID3D11DeviceContext* deviceContext
)
{
	unsigned int stride =
		sizeof(VertexType);

	unsigned int offset = 0;


	deviceContext->IASetVertexBuffers(
		0,
		1,
		&m_VertexBuffer,
		&stride,
		&offset
	);


	deviceContext->IASetIndexBuffer(
		m_IndexBuffer,
		DXGI_FORMAT_R32_UINT,
		0
	);


	deviceContext->IASetPrimitiveTopology(
		D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST
	);

	return;
}