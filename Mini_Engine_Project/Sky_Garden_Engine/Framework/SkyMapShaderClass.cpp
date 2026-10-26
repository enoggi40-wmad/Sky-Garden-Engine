#include "SkyMapShaderClass.h"
#include <Windows.h>
#include <fstream>

using namespace std;
using namespace DirectX;


SkyMapShaderClass::SkyMapShaderClass()
{
	m_VertexShader = 0;
	m_PixelShader = 0;
	m_Layout = 0;
	m_MatrixBuffer = 0;
	m_SampleState = 0;
}


SkyMapShaderClass::~SkyMapShaderClass()
{
}


bool SkyMapShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
	bool result;

	result = InitializeShader(
		device,
		hwnd,
		L"./data/skyMapShader.hlsl"
	);

	if (!result)
	{
		return false;
	}

	return true;
}


void SkyMapShaderClass::Shutdown()
{
	ShutdownShader();

	return;
}


bool SkyMapShaderClass::Render(
	ID3D11DeviceContext* deviceContext,
	int indexCount,
	XMMATRIX worldMatrix,
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* cubeMapTexture
)
{
	

	bool result;

	result = SetShaderParameters(
		deviceContext,
		worldMatrix,
		viewMatrix,
		projectionMatrix,
		cubeMapTexture
	);

	if (!result)
	{
		return false;
	}

	RenderShader(deviceContext, indexCount);

	return true;
}


bool SkyMapShaderClass::InitializeShader(
	ID3D11Device* device,
	HWND hwnd,
	const WCHAR* shaderFilename
)
{
	HRESULT result;

	ID3D10Blob* errorMessage = 0;

	ID3D10Blob* vertexShaderBuffer = 0;
	ID3D10Blob* pixelShaderBuffer = 0;


	// Vertex Shader Compile
	result = D3DCompileFromFile(
		shaderFilename,
		NULL,
		NULL,
		"SKYMAP_VS",
		"vs_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&vertexShaderBuffer,
		&errorMessage
	);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(
				errorMessage,
				hwnd,
				shaderFilename
			);
		}
		else
		{
			MessageBox(
				hwnd,
				shaderFilename,
				L"Missing Shader File",
				MB_OK
			);
		}

		return false;
	}


	// Pixel Shader Compile
	result = D3DCompileFromFile(
		shaderFilename,
		NULL,
		NULL,
		"SKYMAP_PS",
		"ps_5_0",
		D3D10_SHADER_ENABLE_STRICTNESS,
		0,
		&pixelShaderBuffer,
		&errorMessage
	);

	if (FAILED(result))
	{
		if (errorMessage)
		{
			OutputShaderErrorMessage(
				errorMessage,
				hwnd,
				shaderFilename
			);
		}
		else
		{
			MessageBox(
				hwnd,
				shaderFilename,
				L"Missing Shader File",
				MB_OK
			);
		}

		return false;
	}


	// Vertex Shader 생성
	result = device->CreateVertexShader(
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		NULL,
		&m_VertexShader
	);

	if (FAILED(result))
	{
		return false;
	}


	// Pixel Shader 생성
	result = device->CreatePixelShader(
		pixelShaderBuffer->GetBufferPointer(),
		pixelShaderBuffer->GetBufferSize(),
		NULL,
		&m_PixelShader
	);

	if (FAILED(result))
	{
		return false;
	}


	// 교수님 샘플과 같은 Vertex 구조
	D3D11_INPUT_ELEMENT_DESC polygonLayout[3];

	polygonLayout[0].SemanticName = "POSITION";
	polygonLayout[0].SemanticIndex = 0;
	polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[0].InputSlot = 0;
	polygonLayout[0].AlignedByteOffset = 0;
	polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[0].InstanceDataStepRate = 0;

	polygonLayout[1].SemanticName = "TEXCOORD";
	polygonLayout[1].SemanticIndex = 0;
	polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
	polygonLayout[1].InputSlot = 0;
	polygonLayout[1].AlignedByteOffset = 12;
	polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[1].InstanceDataStepRate = 0;

	polygonLayout[2].SemanticName = "NORMAL";
	polygonLayout[2].SemanticIndex = 0;
	polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
	polygonLayout[2].InputSlot = 0;
	polygonLayout[2].AlignedByteOffset = 20;
	polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;
	polygonLayout[2].InstanceDataStepRate = 0;


	unsigned int numElements = 3;


	result = device->CreateInputLayout(
		polygonLayout,
		numElements,
		vertexShaderBuffer->GetBufferPointer(),
		vertexShaderBuffer->GetBufferSize(),
		&m_Layout
	);

	if (FAILED(result))
	{
		return false;
	}


	vertexShaderBuffer->Release();
	vertexShaderBuffer = 0;

	pixelShaderBuffer->Release();
	pixelShaderBuffer = 0;


	// Matrix Constant Buffer
	D3D11_BUFFER_DESC matrixBufferDesc;

	matrixBufferDesc.Usage = D3D11_USAGE_DYNAMIC;
	matrixBufferDesc.ByteWidth = sizeof(MatrixBufferType);
	matrixBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	matrixBufferDesc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
	matrixBufferDesc.MiscFlags = 0;
	matrixBufferDesc.StructureByteStride = 0;


	result = device->CreateBuffer(
		&matrixBufferDesc,
		NULL,
		&m_MatrixBuffer
	);

	if (FAILED(result))
	{
		return false;
	}


	// Sampler State
	D3D11_SAMPLER_DESC samplerDesc;

	samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;

	samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;

	samplerDesc.MipLODBias = 0.0f;
	samplerDesc.MaxAnisotropy = 1;

	samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;

	samplerDesc.BorderColor[0] = 0;
	samplerDesc.BorderColor[1] = 0;
	samplerDesc.BorderColor[2] = 0;
	samplerDesc.BorderColor[3] = 0;

	samplerDesc.MinLOD = 0;
	samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;


	result = device->CreateSamplerState(
		&samplerDesc,
		&m_SampleState
	);

	if (FAILED(result))
	{
		return false;
	}


	return true;
}


void SkyMapShaderClass::ShutdownShader()
{
	if (m_SampleState)
	{
		m_SampleState->Release();
		m_SampleState = 0;
	}

	if (m_MatrixBuffer)
	{
		m_MatrixBuffer->Release();
		m_MatrixBuffer = 0;
	}

	if (m_Layout)
	{
		m_Layout->Release();
		m_Layout = 0;
	}

	if (m_PixelShader)
	{
		m_PixelShader->Release();
		m_PixelShader = 0;
	}

	if (m_VertexShader)
	{
		m_VertexShader->Release();
		m_VertexShader = 0;
	}

	return;
}


bool SkyMapShaderClass::SetShaderParameters(
	ID3D11DeviceContext* deviceContext,
	XMMATRIX worldMatrix,
	XMMATRIX viewMatrix,
	XMMATRIX projectionMatrix,
	ID3D11ShaderResourceView* cubeMapTexture
)
{
	HRESULT result;

	D3D11_MAPPED_SUBRESOURCE mappedResource;

	MatrixBufferType* dataPtr;


	// 교수님 HLSL은 WVP를 하나로 받음
	XMMATRIX wvpMatrix =
		worldMatrix *
		viewMatrix *
		projectionMatrix;


	wvpMatrix = XMMatrixTranspose(wvpMatrix);
	worldMatrix = XMMatrixTranspose(worldMatrix);


	// Matrix Buffer 열기
	result = deviceContext->Map(
		m_MatrixBuffer,
		0,
		D3D11_MAP_WRITE_DISCARD,
		0,
		&mappedResource
	);

	if (FAILED(result))
	{
		return false;
	}


	dataPtr =
		(MatrixBufferType*)mappedResource.pData;

	dataPtr->WVP = wvpMatrix;
	dataPtr->World = worldMatrix;


	deviceContext->Unmap(
		m_MatrixBuffer,
		0
	);


	// Vertex Shader b0
	deviceContext->VSSetConstantBuffers(
		0,
		1,
		&m_MatrixBuffer
	);


	// Pixel Shader에 CubeMap 전달
	deviceContext->PSSetShaderResources(
		0,
		1,
		&cubeMapTexture
	);


	return true;
}


void SkyMapShaderClass::RenderShader(
	ID3D11DeviceContext* deviceContext,
	int indexCount
)
{
	deviceContext->IASetInputLayout(
		m_Layout
	);


	deviceContext->VSSetShader(
		m_VertexShader,
		NULL,
		0
	);

	deviceContext->PSSetShader(
		m_PixelShader,
		NULL,
		0
	);


	deviceContext->PSSetSamplers(
		0,
		1,
		&m_SampleState
	);


	deviceContext->DrawIndexed(
		indexCount,
		0,
		0
	);

	return;
}


void SkyMapShaderClass::OutputShaderErrorMessage(
	ID3D10Blob* errorMessage,
	HWND hwnd,
	const WCHAR* shaderFilename
)
{
	char* compileErrors;

	unsigned long long bufferSize;

	ofstream fout;


	compileErrors =
		(char*)(errorMessage->GetBufferPointer());

	bufferSize =
		errorMessage->GetBufferSize();


	fout.open("shader-error.txt");

	for (unsigned long long i = 0; i < bufferSize; i++)
	{
		fout << compileErrors[i];
	}

	fout.close();


	errorMessage->Release();
	errorMessage = 0;


	MessageBox(
		hwnd,
		L"Error compiling shader. Check shader-error.txt.",
		shaderFilename,
		MB_OK
	);

	return;
}