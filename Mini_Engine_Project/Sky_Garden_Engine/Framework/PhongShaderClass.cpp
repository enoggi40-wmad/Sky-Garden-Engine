#include "PhongShaderClass.h"

PhongShaderClass::PhongShaderClass()
    : m_vertexShader(nullptr),
    m_pixelShader(nullptr),
    m_layout(nullptr),
    m_sampleState(nullptr),
    m_matrixBuffer(nullptr),
    m_lightBuffer(nullptr),
    m_cameraBuffer(nullptr),
    m_lightDataBuffer(nullptr)
{
}

PhongShaderClass::PhongShaderClass(const PhongShaderClass& other)
    : PhongShaderClass()
{
    // 기존 헤더의 복사 생성자 선언에 대응.
    // GPU 리소스를 소유하는 클래스이므로 실제 복사는 사용하지 않는 것이 안전함.
}

PhongShaderClass::~PhongShaderClass()
{
    // 기존 엔진과 동일하게 ShaderManager에서 Shutdown()을 호출하는 구조.
}

bool PhongShaderClass::Initialize(ID3D11Device* device, HWND hwnd)
{
    return InitializeShader(device, hwnd, L"./data/phongShader.hlsl");
}

void PhongShaderClass::Shutdown()
{
    ShutdownShader();
}

bool PhongShaderClass::Render(
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
    const LightData& lightData)
{
    if (!SetShaderParameters(
        deviceContext,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        texture,
        cameraPosition,
        ambientColor,
        specularColor,
        specularPower,
        lightData))
    {
        return false;
    }

    RenderShader(deviceContext, indexCount);
    return true;
}

bool PhongShaderClass::InitializeShader(
    ID3D11Device* device,
    HWND hwnd,
    const WCHAR* fileName)
{
    HRESULT result;
    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vertexShaderBuffer = nullptr;
    ID3D10Blob* pixelShaderBuffer = nullptr;

    // 기존 HLSL 진입점 이름 유지
    result = D3DCompileFromFile(
        fileName, nullptr, nullptr,
        "LightVertexShader", "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &vertexShaderBuffer, &errorMessage);

    if (FAILED(result))
    {
        if (errorMessage)
            OutputShaderErrorMessage(errorMessage, hwnd, fileName);
        else
            MessageBox(hwnd, fileName, L"Missing Shader File", MB_OK);
        return false;
    }

    result = D3DCompileFromFile(
        fileName, nullptr, nullptr,
        "LightPixelShader", "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS, 0,
        &pixelShaderBuffer, &errorMessage);

    if (FAILED(result))
    {
        if (errorMessage)
            OutputShaderErrorMessage(errorMessage, hwnd, fileName);
        else
            MessageBox(hwnd, fileName, L"Missing Shader File", MB_OK);

        vertexShaderBuffer->Release();
        return false;
    }

    result = device->CreateVertexShader(
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        nullptr, &m_vertexShader);

    if (FAILED(result))
    {
        vertexShaderBuffer->Release();
        pixelShaderBuffer->Release();
        return false;
    }

    result = device->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(),
        nullptr, &m_pixelShader);

    if (FAILED(result))
    {
        vertexShaderBuffer->Release();
        pixelShaderBuffer->Release();
        ShutdownShader();
        return false;
    }

    // ModelClass의 VertexType: POSITION, TEXCOORD, NORMAL
    D3D11_INPUT_ELEMENT_DESC polygonLayout[3] = {};

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format = DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format = DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset = D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass = D3D11_INPUT_PER_VERTEX_DATA;

    result = device->CreateInputLayout(
        polygonLayout, 3,
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        &m_layout);

    vertexShaderBuffer->Release();
    pixelShaderBuffer->Release();

    if (FAILED(result))
    {
        ShutdownShader();
        return false;
    }

    D3D11_SAMPLER_DESC samplerDesc = {};
    samplerDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
    samplerDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
    samplerDesc.ComparisonFunc = D3D11_COMPARISON_ALWAYS;
    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MaxAnisotropy = 1;

    result = device->CreateSamplerState(&samplerDesc, &m_sampleState);
    if (FAILED(result))
    {
        ShutdownShader();
        return false;
    }

    // VS b0: MatrixBuffer
    // VS b1: CameraBuffer
    // PS b0: LightBuffer (Ambient + Material Specular)
    // PS b1: LightDataBuffer (Directional + Point + Spot)
    auto createDynamicBuffer = [device](UINT byteWidth, ID3D11Buffer** buffer) -> bool
        {
            D3D11_BUFFER_DESC desc = {};
            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = byteWidth;
            desc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
            desc.CPUAccessFlags = D3D11_CPU_ACCESS_WRITE;
            return SUCCEEDED(device->CreateBuffer(&desc, nullptr, buffer));
        };

    if (!createDynamicBuffer(sizeof(MatrixBufferType), &m_matrixBuffer) ||
        !createDynamicBuffer(sizeof(CameraBufferType), &m_cameraBuffer) ||
        !createDynamicBuffer(sizeof(LightBufferType), &m_lightBuffer) ||
        !createDynamicBuffer(sizeof(LightData), &m_lightDataBuffer))
    {
        ShutdownShader();
        return false;
    }

    return true;
}

void PhongShaderClass::ShutdownShader()
{
    if (m_lightDataBuffer)
    {
        m_lightDataBuffer->Release();
        m_lightDataBuffer = nullptr;
    }

    if (m_lightBuffer)
    {
        m_lightBuffer->Release();
        m_lightBuffer = nullptr;
    }

    if (m_cameraBuffer)
    {
        m_cameraBuffer->Release();
        m_cameraBuffer = nullptr;
    }

    if (m_matrixBuffer)
    {
        m_matrixBuffer->Release();
        m_matrixBuffer = nullptr;
    }

    if (m_sampleState)
    {
        m_sampleState->Release();
        m_sampleState = nullptr;
    }

    if (m_layout)
    {
        m_layout->Release();
        m_layout = nullptr;
    }

    if (m_pixelShader)
    {
        m_pixelShader->Release();
        m_pixelShader = nullptr;
    }

    if (m_vertexShader)
    {
        m_vertexShader->Release();
        m_vertexShader = nullptr;
    }
}

void PhongShaderClass::OutputShaderErrorMessage(
    ID3D10Blob* errorMessage,
    HWND hwnd,
    const WCHAR* shaderFilename)
{
    char* compileErrors = static_cast<char*>(errorMessage->GetBufferPointer());
    unsigned long bufferSize = static_cast<unsigned long>(errorMessage->GetBufferSize());

    ofstream fout;
    fout.open("shader-error.txt");
    for (unsigned long i = 0; i < bufferSize; ++i)
    {
        fout << compileErrors[i];
    }
    fout.close();

    errorMessage->Release();
    MessageBox(hwnd,
        L"Error compiling shader. Check shader-error.txt for message.",
        shaderFilename, MB_OK);
}

bool PhongShaderClass::SetShaderParameters(
    ID3D11DeviceContext* deviceContext,
    XMMATRIX worldMatrix,
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix,
    ID3D11ShaderResourceView* texture,
    XMFLOAT3 cameraPosition,
    XMFLOAT4 ambientColor,
    XMFLOAT4 specularColor,
    float specularPower,
    const LightData& lightData)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    // 기존 엔진의 행벡터 World * View * Projection 규약 유지
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix = XMMatrixTranspose(projectionMatrix);

    // VS b0: 행렬
    result = deviceContext->Map(
        m_matrixBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    MatrixBufferType* matrixPtr =
        static_cast<MatrixBufferType*>(mappedResource.pData);
    matrixPtr->world = worldMatrix;
    matrixPtr->view = viewMatrix;
    matrixPtr->projection = projectionMatrix;
    deviceContext->Unmap(m_matrixBuffer, 0);
    deviceContext->VSSetConstantBuffers(0, 1, &m_matrixBuffer);

    // PS t0: Diffuse Texture
    deviceContext->PSSetShaderResources(0, 1, &texture);

    // VS b1: 카메라 위치
    result = deviceContext->Map(
        m_cameraBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    CameraBufferType* cameraPtr =
        static_cast<CameraBufferType*>(mappedResource.pData);
    cameraPtr->cameraPosition = cameraPosition;
    cameraPtr->padding = 0.0f;
    deviceContext->Unmap(m_cameraBuffer, 0);
    deviceContext->VSSetConstantBuffers(1, 1, &m_cameraBuffer);

    // PS b0: Ambient 및 Material의 Specular 속성
    result = deviceContext->Map(
        m_lightBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    LightBufferType* lightPtr =
        static_cast<LightBufferType*>(mappedResource.pData);
    lightPtr->ambientColor = ambientColor;
    lightPtr->specularColor = specularColor;
    lightPtr->specularPower = specularPower;
    lightPtr->padding[0] = 0.0f;
    lightPtr->padding[1] = 0.0f;
    lightPtr->padding[2] = 0.0f;
    deviceContext->Unmap(m_lightBuffer, 0);
    deviceContext->PSSetConstantBuffers(0, 1, &m_lightBuffer);

    // PS b1: Scene에 등록된 모든 광원 데이터
    result = deviceContext->Map(
        m_lightDataBuffer, 0, D3D11_MAP_WRITE_DISCARD, 0, &mappedResource);
    if (FAILED(result))
        return false;

    LightData* lightDataPtr =
        static_cast<LightData*>(mappedResource.pData);
    *lightDataPtr = lightData;
    deviceContext->Unmap(m_lightDataBuffer, 0);
    deviceContext->PSSetConstantBuffers(1, 1, &m_lightDataBuffer);

    return true;
}

void PhongShaderClass::RenderShader(
    ID3D11DeviceContext* deviceContext,
    int indexCount)
{
    deviceContext->IASetInputLayout(m_layout);
    deviceContext->VSSetShader(m_vertexShader, nullptr, 0);
    deviceContext->PSSetShader(m_pixelShader, nullptr, 0);
    deviceContext->PSSetSamplers(0, 1, &m_sampleState);
    deviceContext->DrawIndexed(indexCount, 0, 0);
}
