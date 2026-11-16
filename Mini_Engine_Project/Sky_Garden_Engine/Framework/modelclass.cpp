////////////////////////////////////////////////////////////////////////////////
// Filename: modelclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "modelclass.h"
#include <vector>
#include <cmath>


ModelClass::ModelClass()
{
	m_vertexBuffer = 0;
	m_indexBuffer = 0;
	
	m_model = 0;

	
}


ModelClass::ModelClass(const ModelClass& other)
{
}


ModelClass::~ModelClass()
{
}


bool ModelClass::Initialize(
	ID3D11Device* device,
	const std::vector<ModelVertexData>& modelData)
{
	bool result;


	// 모델 데이터 개수 설정
	m_vertexCount =
		static_cast<int>(modelData.size());

	m_indexCount =
		m_vertexCount;


	// ModelClass 내부 데이터 생성
	m_model =
		new ModelVertexData[m_vertexCount];

	if (!m_model)
	{
		return false;
	}


	// Loader에서 받은 모델 데이터 복사
	for (int i = 0; i < m_vertexCount; i++)
	{
		m_model[i] =
			modelData[i];
	}


	// Vertex / Index Buffer 생성
	result =
		InitializeBuffers(device);

	if (!result)
	{
		return false;
	}

	return true;
}


void ModelClass::Shutdown()
{
	

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	if (m_model)
	{
		delete[] m_model;
		m_model = 0;
	}
	
	return;
}


void ModelClass::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);

	return;
}


int ModelClass::GetIndexCount()
{
	return m_indexCount;
}






// 삼각형의 위치·UV를 이용해 각 정점의 접선 계산 -> 추후 MaterialClass로 옮길 예정
void ModelClass::CalculateTangents(VertexType* vertices)
{
	const float epsilon = 0.000001f;

	// 정점 3개 = 삼각형 1개
	for (int i = 0; i + 2 < m_vertexCount; i += 3)
	{
		VertexType& v0 = vertices[i];
		VertexType& v1 = vertices[i + 1];
		VertexType& v2 = vertices[i + 2];

		// 삼각형의 두 변
		XMVECTOR p0 = XMLoadFloat3(&v0.position);
		XMVECTOR p1 = XMLoadFloat3(&v1.position);
		XMVECTOR p2 = XMLoadFloat3(&v2.position);

		XMVECTOR edge1 = XMVectorSubtract(p1, p0);
		XMVECTOR edge2 = XMVectorSubtract(p2, p0);

		// 두 변에 대응하는 UV 변화량
		float du1 = v1.texture.x - v0.texture.x;
		float dv1 = v1.texture.y - v0.texture.y;

		float du2 = v2.texture.x - v0.texture.x;
		float dv2 = v2.texture.y - v0.texture.y;

		float determinant = du1 * dv2 - du2 * dv1;
		bool validUV = std::fabs(determinant) > epsilon;

		XMVECTOR tangent = XMVectorZero();
		XMVECTOR binormal = XMVectorZero();

		if (validUV)
		{
			float inverse = 1.0f / determinant;

			// UV의 U 방향에 대응하는 접선
			tangent = XMVectorScale(
				XMVectorSubtract(
					XMVectorScale(edge1, dv2),
					XMVectorScale(edge2, dv1)
				),
				inverse
			);

			// UV의 V 방향에 대응하는 접선
			binormal = XMVectorScale(
				XMVectorSubtract(
					XMVectorScale(edge2, du1),
					XMVectorScale(edge1, du2)
				),
				inverse
			);
		}

		// 각 정점의 기존 법선에 맞춰 접선을 정리
		for (int j = 0; j < 3; ++j)
		{
			VertexType& vertex = vertices[i + j];

			XMVECTOR normal = XMLoadFloat3(&vertex.normal);

			// 법선이 없으면 삼각형의 면 법선 사용
			if (XMVectorGetX(XMVector3LengthSq(normal)) < epsilon)
			{
				normal = XMVector3Cross(edge1, edge2);
			}

			// 면적이 0인 삼각형에 대한 예외 처리
			if (XMVectorGetX(XMVector3LengthSq(normal)) < epsilon)
			{
				normal = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);
			}

			normal = XMVector3Normalize(normal);

			// 탄젠트에서 법선 방향 성분 제거 → 서로 수직으로 만듦
			float projection =
				XMVectorGetX(XMVector3Dot(normal, tangent));

			XMVECTOR finalTangent = XMVectorSubtract(
				tangent,
				XMVectorScale(normal, projection)
			);

			// UV가 겹치거나 접선 계산이 불가능하면 임시 접선 생성
			if (!validUV ||
				XMVectorGetX(XMVector3LengthSq(finalTangent)) < epsilon)
			{
				XMVECTOR reference =
					std::fabs(XMVectorGetY(normal)) < 0.999f
					? XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f)
					: XMVectorSet(1.0f, 0.0f, 0.0f, 0.0f);

				finalTangent = XMVector3Cross(reference, normal);
			}

			finalTangent = XMVector3Normalize(finalTangent);

			XMVECTOR finalBinormal =
				XMVector3Cross(normal, finalTangent);

			// 뒤집힌 UV에서도 원래 V 방향을 유지
			if (validUV &&
				XMVectorGetX(
					XMVector3Dot(finalBinormal, binormal)
				) < 0.0f)
			{
				finalBinormal = XMVectorNegate(finalBinormal);
			}

			finalBinormal = XMVector3Normalize(finalBinormal);

			XMStoreFloat3(&vertex.normal, normal);
			XMStoreFloat3(&vertex.tangent, finalTangent);
			XMStoreFloat3(&vertex.binormal, finalBinormal);
		}
	}
}



bool ModelClass::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
    D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create the vertex array.
	vertices = new VertexType[m_vertexCount];
	if(!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[m_indexCount];
	if(!indices)
	{
		return false;
	}

	// Load the vertex array and index array with data.
	for (i = 0; i < m_vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(m_model[i].x, m_model[i].y, m_model[i].z);
		vertices[i].texture = XMFLOAT2(m_model[i].tu, m_model[i].tv);
		vertices[i].normal = XMFLOAT3(m_model[i].nx, m_model[i].ny, m_model[i].nz);

		indices[i] = i;
	}

	//////////////////////////////////////////////////////////////////////
	// Calculate the tangent and binormal vectors for the model.
	CalculateTangents(vertices);

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;
	/////////////////////////////////////////////////////////////////////


	// Set up the description of the static vertex buffer.
    vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    vertexBufferDesc.ByteWidth = sizeof(VertexType) * m_vertexCount;
    vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
    vertexBufferDesc.CPUAccessFlags = 0;
    vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the vertex data.
    vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
    result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &m_vertexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
    indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
    indexBufferDesc.ByteWidth = sizeof(unsigned long) * m_indexCount;
    indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
    indexBufferDesc.CPUAccessFlags = 0;
    indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
    indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &m_indexBuffer);
	if(FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete [] vertices;
	vertices = 0;

	delete [] indices;
	indices = 0;

	return true;
}


void ModelClass::ShutdownBuffers()
{
	// Release the index buffer.
	if(m_indexBuffer)
	{
		m_indexBuffer->Release();
		m_indexBuffer = 0;
	}

	// Release the vertex buffer.
	if(m_vertexBuffer)
	{
		m_vertexBuffer->Release();
		m_vertexBuffer = 0;
	}

	return;
}


void ModelClass::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;


	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType); 
	offset = 0;
    
	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &m_vertexBuffer, &stride, &offset);

    // Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(m_indexBuffer, DXGI_FORMAT_R32_UINT, 0);

    // Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	return;
}


