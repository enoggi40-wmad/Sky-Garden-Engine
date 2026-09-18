////////////////////////////////////////////////////////////////////////////////
// Filename: textclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textclass.h"


TextClass::TextClass()
{
	m_Font = 0;
	m_FontShader = 0;

	m_sentence1 = 0;
	m_sentence2 = 0;
	m_sentence3 = 0;
}


TextClass::TextClass(const TextClass& other)
{
}


TextClass::~TextClass()
{
}


bool TextClass::Initialize(
	ID3D11Device* device,
	ID3D11DeviceContext* deviceContext,
	HWND hwnd,
	int screenWidth,
	int screenHeight,
	XMMATRIX baseViewMatrix)
{
	bool result;

	// Store the screen width and height.
	m_screenWidth = screenWidth;
	m_screenHeight = screenHeight;

	// Store the base view matrix.
	m_baseViewMatrix = baseViewMatrix;

	// Create the font object.
	m_Font = new FontClass;
	if (!m_Font)
	{
		return false;
	}

	// Initialize the font object.
	result = m_Font->Initialize(device, L"./data/fontdata.txt", L"./data/font.dds");
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font object.", L"Error", MB_OK);
		return false;
	}

	// Create the font shader object.
	m_FontShader = new FontShaderClass;
	if (!m_FontShader)
	{
		return false;
	}

	// Initialize the font shader object.
	result = m_FontShader->Initialize(device, hwnd);
	if (!result)
	{
		MessageBox(hwnd, L"Could not initialize the font shader object.", L"Error", MB_OK);
		return false;
	}

	// Initialize 3 sentences.
	result = InitializeSentence(&m_sentence1, 256, device);
	if (!result)
	{
		return false;
	}

	// Initialize default text once.
	result = SetPerformanceInfo(0, 0, 0, 0, screenWidth, screenHeight, deviceContext);
	if (!result)
	{
		return false;
	}

	return true;
}


void TextClass::Shutdown()
{
	// Release sentences.
	ReleaseSentence(&m_sentence1);
	ReleaseSentence(&m_sentence2);
	ReleaseSentence(&m_sentence3);

	// Release the font shader object.
	if (m_FontShader)
	{
		m_FontShader->Shutdown();
		delete m_FontShader;
		m_FontShader = 0;
	}

	// Release the font object.
	if (m_Font)
	{
		m_Font->Shutdown();
		delete m_Font;
		m_Font = 0;
	}

	return;
}


bool TextClass::Render(ID3D11DeviceContext* deviceContext, XMMATRIX worldMatrix, XMMATRIX orthoMatrix)
{
	bool result;

	result = RenderSentence(deviceContext, m_sentence1, worldMatrix, orthoMatrix);
	if (!result)
	{
		return false;
	}

	return true;
}


bool TextClass::InitializeSentence(SentenceType** sentence, int maxLength, ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	int i;

	// Create a new sentence object.
	*sentence = new SentenceType;
	if (!*sentence)
	{
		return false;
	}

	// Initialize the sentence buffers to null.
	(*sentence)->vertexBuffer = 0;
	(*sentence)->indexBuffer = 0;

	// Set sentence information.
	(*sentence)->maxLength = maxLength;
	(*sentence)->vertexCount = 6 * maxLength;
	(*sentence)->indexCount = (*sentence)->vertexCount;
	(*sentence)->currentIndexCount = 0;

	(*sentence)->red = 1.0f;
	(*sentence)->green = 1.0f;
	(*sentence)->blue = 1.0f;

	// Create the vertex array.
	vertices = new VertexType[(*sentence)->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[(*sentence)->indexCount];
	if (!indices)
	{
		delete[] vertices;
		vertices = 0;
		return false;
	}

	// Initialize vertex array to zeros.
	memset(vertices, 0, sizeof(VertexType) * (*sentence)->vertexCount);

	// Initialize the index array.
	for (i = 0; i < (*sentence)->indexCount; i++)
	{
		indices[i] = i;
	}

	// Set up the dynamic vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * (*sentence)->vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &(*sentence)->vertexBuffer);
	if (FAILED(result))
	{
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;

		return false;
	}

	// Set up the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * (*sentence)->indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	result = device->CreateBuffer(&indexBufferDesc, &indexData, &(*sentence)->indexBuffer);
	if (FAILED(result))
	{
		delete[] vertices;
		vertices = 0;

		delete[] indices;
		indices = 0;

		return false;
	}

	// Release arrays.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}


bool TextClass::UpdateSentence(
	SentenceType* sentence,
	const char* text,
	int positionX,
	int positionY,
	float red,
	float green,
	float blue,
	ID3D11DeviceContext* deviceContext)
{
	int numLetters;
	VertexType* vertices;
	float drawX, drawY;
	HRESULT result;
	D3D11_MAPPED_SUBRESOURCE mappedResource;
	VertexType* verticesPtr;

	if (!sentence || !text)
	{
		return false;
	}

	// Store sentence color.
	sentence->red = red;
	sentence->green = green;
	sentence->blue = blue;

	// Get string length.
	numLetters = (int)strlen(text);

	// Check for overflow.
	if (numLetters > sentence->maxLength)
	{
		return false;
	}

	// 실제 글자 수만큼만 렌더링하도록 저장한다.
	sentence->currentIndexCount = numLetters * 6;

	// Create vertex array.
	vertices = new VertexType[sentence->vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Clear vertex array.
	memset(vertices, 0, sizeof(VertexType) * sentence->vertexCount);

	// Calculate screen position.
	drawX = (float)(((m_screenWidth / 2) * -1) + positionX);
	drawY = (float)((m_screenHeight / 2) - positionY);

	// Build vertex array from text.
	m_Font->BuildVertexArray((void*)vertices, text, drawX, drawY);

	// Lock vertex buffer.
	result = deviceContext->Map(sentence->vertexBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
	if (FAILED(result))
	{
		delete[] vertices;
		vertices = 0;
		return false;
	}

	// Copy data.
	verticesPtr = (VertexType*)mappedResource.pData;
	memcpy(verticesPtr, vertices, sizeof(VertexType) * sentence->vertexCount);

	// Unlock vertex buffer.
	deviceContext->Unmap(sentence->vertexBuffer, 0);

	// Release vertex array.
	delete[] vertices;
	vertices = 0;

	return true;
}


void TextClass::ReleaseSentence(SentenceType** sentence)
{
	if (*sentence)
	{
		if ((*sentence)->vertexBuffer)
		{
			(*sentence)->vertexBuffer->Release();
			(*sentence)->vertexBuffer = 0;
		}

		if ((*sentence)->indexBuffer)
		{
			(*sentence)->indexBuffer->Release();
			(*sentence)->indexBuffer = 0;
		}

		delete* sentence;
		*sentence = 0;
	}

	return;
}


bool TextClass::RenderSentence(
	ID3D11DeviceContext* deviceContext,
	SentenceType* sentence,
	XMMATRIX worldMatrix,
	XMMATRIX orthoMatrix)
{
	unsigned int stride, offset;
	XMFLOAT4 pixelColor;
	bool result;

	if (!sentence)
	{
		return false;
	}

	if (sentence->currentIndexCount <= 0)
	{
		return true;
	}

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set buffers.
	deviceContext->IASetVertexBuffers(0, 1, &sentence->vertexBuffer, &stride, &offset);
	deviceContext->IASetIndexBuffer(sentence->indexBuffer, DXGI_FORMAT_R32_UINT, 0);
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	// Set color.
	pixelColor = XMFLOAT4(sentence->red, sentence->green, sentence->blue, 1.0f);

	// Render actual text indices only.
	result = m_FontShader->Render(
		deviceContext,
		sentence->currentIndexCount,
		worldMatrix,
		m_baseViewMatrix,
		orthoMatrix,
		m_Font->GetTexture(),
		pixelColor);

	if (!result)
	{
		return false;
	}

	return true;
}


bool TextClass::SetPerformanceInfo(
	int fps,
	int cpu,
	int objectCount,
	int polygonCount,
	int screenWidth,
	int screenHeight,
	ID3D11DeviceContext* deviceContext)
{
	char infoString[256];
	bool result;

	if (fps > 9999)
	{
		fps = 9999;
	}

	sprintf_s(
		infoString,
		"FPS=%d   CPU=%d%%   OBJ=%d   POLY=%d   RES=%dx%d",
		fps,
		cpu,
		objectCount,
		polygonCount,
		screenWidth,
		screenHeight
	);

	result = UpdateSentence(
		m_sentence1,
		infoString,
		20,
		25,
		0.05f,
		0.15f,
		0.35f,
		deviceContext);

	if (!result)
	{
		return false;
	}

	return true;
}