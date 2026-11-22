
#include "BumpMapShaderClass.h"

BumpMapShaderClass::BumpMapShaderClass()
{
}

BumpMapShaderClass::BumpMapShaderClass(
    const BumpMapShaderClass& other)
{
    // GPU 리소스를 소유하므로 복사 사용은 피해야 함
}

BumpMapShaderClass::~BumpMapShaderClass()
{
}

bool BumpMapShaderClass::Initialize(
    ID3D11Device* device,
    HWND hwnd)
{
    return InitializeShader(
        device,
        hwnd,
        L"./data/bumpmapShader.hlsl"
    );
}

void BumpMapShaderClass::Shutdown()
{
    ShutdownShader();
}

bool BumpMapShaderClass::Render(
    ID3D11DeviceContext* deviceContext,
    int indexCount,
    XMMATRIX worldMatrix,
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix,
    ID3D11ShaderResourceView** textureArray,
    XMFLOAT3 cameraPosition,
    XMFLOAT4 ambientColor,
    XMFLOAT4 specularColor,
    float specularPower,
    XMFLOAT4 emissiveColor,
    float emissiveIntensity,
    const LightData& lightData)
{
    if (!SetShaderParameters(
        deviceContext,
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        textureArray,
        cameraPosition,
        ambientColor,
        specularColor,
        specularPower,
        emissiveColor,
        emissiveIntensity,
        lightData))
    {
        return false;
    }

    RenderShader(deviceContext, indexCount);
    return true;
}

bool BumpMapShaderClass::InitializeShader(
    ID3D11Device* device,
    HWND hwnd,
    const WCHAR* fileName)
{
    HRESULT result;

    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vertexShaderBuffer = nullptr;
    ID3D10Blob* pixelShaderBuffer = nullptr;

    // ==========================================
    // Vertex Shader 컴파일
    // ==========================================

    result = D3DCompileFromFile(
        fileName,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "BumpMapVertexShader",
        "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        &vertexShaderBuffer,
        &errorMessage
    );

    if (FAILED(result) || !vertexShaderBuffer)
    {
        if (errorMessage)
            OutputShaderErrorMessage(
                errorMessage, hwnd, fileName);
        else
            MessageBox(
                hwnd,
                fileName,
                L"Vertex Shader Compile Failed",
                MB_OK
            );

        return false;
    }

    if (errorMessage)
    {
        errorMessage->Release();
        errorMessage = nullptr;
    }

    // ==========================================
    // Pixel Shader 컴파일
    // ==========================================

    result = D3DCompileFromFile(
        fileName,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "BumpMapPixelShader",
        "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        &pixelShaderBuffer,
        &errorMessage
    );

    if (FAILED(result) || !pixelShaderBuffer)
    {
        if (errorMessage)
            OutputShaderErrorMessage(
                errorMessage, hwnd, fileName);
        else
            MessageBox(
                hwnd,
                fileName,
                L"Pixel Shader Compile Failed",
                MB_OK
            );

        vertexShaderBuffer->Release();
        return false;
    }

    if (errorMessage)
    {
        errorMessage->Release();
        errorMessage = nullptr;
    }

    // ==========================================
    // GPU Shader 생성
    // ==========================================

    result = device->CreateVertexShader(
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        nullptr,
        &m_vertexShader
    );

    if (FAILED(result))
    {
        vertexShaderBuffer->Release();
        pixelShaderBuffer->Release();
        ShutdownShader();
        return false;
    }

    result = device->CreatePixelShader(
        pixelShaderBuffer->GetBufferPointer(),
        pixelShaderBuffer->GetBufferSize(),
        nullptr,
        &m_pixelShader
    );

    if (FAILED(result))
    {
        vertexShaderBuffer->Release();
        pixelShaderBuffer->Release();
        ShutdownShader();
        return false;
    }

    // ==========================================
    // Input Layout
    // 기존 5개 Semantic 유지
    // ==========================================

    D3D11_INPUT_ELEMENT_DESC polygonLayout[5] = {};

    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].Format =
        DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    polygonLayout[3].SemanticName = "TANGENT";
    polygonLayout[3].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[3].InputSlot = 0;
    polygonLayout[3].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[3].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    polygonLayout[4].SemanticName = "BINORMAL";
    polygonLayout[4].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[4].InputSlot = 0;
    polygonLayout[4].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[4].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    result = device->CreateInputLayout(
        polygonLayout,
        5,
        vertexShaderBuffer->GetBufferPointer(),
        vertexShaderBuffer->GetBufferSize(),
        &m_layout
    );

    vertexShaderBuffer->Release();
    pixelShaderBuffer->Release();

    if (FAILED(result))
    {
        ShutdownShader();
        return false;
    }

    // ==========================================
    // Sampler 생성
    // ==========================================

    D3D11_SAMPLER_DESC samplerDesc = {};

    samplerDesc.Filter =
        D3D11_FILTER_MIN_MAG_MIP_LINEAR;

    samplerDesc.AddressU =
        D3D11_TEXTURE_ADDRESS_WRAP;

    samplerDesc.AddressV =
        D3D11_TEXTURE_ADDRESS_WRAP;

    samplerDesc.AddressW =
        D3D11_TEXTURE_ADDRESS_WRAP;

    samplerDesc.ComparisonFunc =
        D3D11_COMPARISON_ALWAYS;

    samplerDesc.MinLOD = 0;
    samplerDesc.MaxLOD = D3D11_FLOAT32_MAX;
    samplerDesc.MaxAnisotropy = 1;

    result = device->CreateSamplerState(
        &samplerDesc,
        &m_sampleState
    );

    if (FAILED(result))
    {
        ShutdownShader();
        return false;
    }

    // ==========================================
    // Constant Buffer 생성
    // ==========================================

    auto createDynamicBuffer =
        [device](
            UINT byteWidth,
            ID3D11Buffer** buffer) -> bool
        {
            D3D11_BUFFER_DESC desc = {};

            desc.Usage = D3D11_USAGE_DYNAMIC;
            desc.ByteWidth = byteWidth;
            desc.BindFlags =
                D3D11_BIND_CONSTANT_BUFFER;

            desc.CPUAccessFlags =
                D3D11_CPU_ACCESS_WRITE;

            return SUCCEEDED(
                device->CreateBuffer(
                    &desc, nullptr, buffer)
            );
        };

    if (!createDynamicBuffer(
        sizeof(MatrixBufferType),
        &m_matrixBuffer) ||

        !createDynamicBuffer(
            sizeof(CameraBufferType),
            &m_cameraBuffer) ||

        !createDynamicBuffer(
            sizeof(LightBufferType),
            &m_lightBuffer) ||

        !createDynamicBuffer(
            sizeof(LightData),
            &m_lightDataBuffer))
    {
        ShutdownShader();
        return false;
    }

    return true;
}

void BumpMapShaderClass::ShutdownShader()
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

void BumpMapShaderClass::OutputShaderErrorMessage(
    ID3D10Blob* errorMessage,
    HWND hwnd,
    const WCHAR* shaderFilename)
{
    OutputDebugStringA(
        static_cast<const char*>(
            errorMessage->GetBufferPointer()
            )
    );

    errorMessage->Release();

    MessageBox(
        hwnd,
        L"Error compiling shader. Check Debug Output.",
        shaderFilename,
        MB_OK
    );
}

bool BumpMapShaderClass::SetShaderParameters(
    ID3D11DeviceContext* deviceContext,
    XMMATRIX worldMatrix,
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix,
    ID3D11ShaderResourceView** textureArray,
    XMFLOAT3 cameraPosition,
    XMFLOAT4 ambientColor,
    XMFLOAT4 specularColor,
    float specularPower,
    XMFLOAT4 emissiveColor,
    float emissiveIntensity,
    const LightData& lightData)
{
    HRESULT result;
    D3D11_MAPPED_SUBRESOURCE mappedResource = {};

    // 기존 DirectXMath 행렬 규약 유지
    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix =
        XMMatrixTranspose(projectionMatrix);

    // ==========================================
    // VS b0: MatrixBuffer
    // ==========================================

    result = deviceContext->Map(
        m_matrixBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource
    );

    if (FAILED(result))
        return false;

    MatrixBufferType* matrixPtr =
        static_cast<MatrixBufferType*>(
            mappedResource.pData
            );

    matrixPtr->world = worldMatrix;
    matrixPtr->view = viewMatrix;
    matrixPtr->projection = projectionMatrix;

    deviceContext->Unmap(m_matrixBuffer, 0);

    deviceContext->VSSetConstantBuffers(
        0, 1, &m_matrixBuffer
    );

    // ==========================================
    // VS b1: CameraBuffer
    // ==========================================

    result = deviceContext->Map(
        m_cameraBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource
    );

    if (FAILED(result))
        return false;

    CameraBufferType* cameraPtr =
        static_cast<CameraBufferType*>(
            mappedResource.pData
            );

    cameraPtr->cameraPosition = cameraPosition;
    cameraPtr->padding = 0.0f;

    deviceContext->Unmap(m_cameraBuffer, 0);

    deviceContext->VSSetConstantBuffers(
        1, 1, &m_cameraBuffer
    );

    // ==========================================
    // PS t0, t1: Color Texture + Normal Map
    // ==========================================

    deviceContext->PSSetShaderResources(
        0, 2, textureArray
    );

    // ==========================================
    // PS b0: Material Lighting
    // ==========================================

    result = deviceContext->Map(
        m_lightBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource
    );

    if (FAILED(result))
        return false;

    LightBufferType* lightPtr =
        static_cast<LightBufferType*>(
            mappedResource.pData
            );

    lightPtr->ambientColor = ambientColor;
    lightPtr->specularColor = specularColor;
    lightPtr->specularPower = specularPower;

    lightPtr->padding[0] = 0.0f;
    lightPtr->padding[1] = 0.0f;
    lightPtr->padding[2] = 0.0f;

    lightPtr->emissiveColor = emissiveColor;
    lightPtr->emissiveIntensity =
        emissiveIntensity;

    lightPtr->emissivePadding[0] = 0.0f;
    lightPtr->emissivePadding[1] = 0.0f;
    lightPtr->emissivePadding[2] = 0.0f;

    deviceContext->Unmap(m_lightBuffer, 0);

    deviceContext->PSSetConstantBuffers(
        0, 1, &m_lightBuffer
    );

    // ==========================================
    // PS b1: Scene LightData
    // ==========================================

    result = deviceContext->Map(
        m_lightDataBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource
    );

    if (FAILED(result))
        return false;

    LightData* lightDataPtr =
        static_cast<LightData*>(
            mappedResource.pData
            );

    *lightDataPtr = lightData;

    deviceContext->Unmap(m_lightDataBuffer, 0);

    deviceContext->PSSetConstantBuffers(
        1, 1, &m_lightDataBuffer
    );

    return true;
}

void BumpMapShaderClass::RenderShader(
    ID3D11DeviceContext* deviceContext,
    int indexCount)
{
    deviceContext->IASetInputLayout(m_layout);

    deviceContext->VSSetShader(
        m_vertexShader, nullptr, 0
    );

    deviceContext->PSSetShader(
        m_pixelShader, nullptr, 0
    );

    deviceContext->PSSetSamplers(
        0, 1, &m_sampleState
    );

    deviceContext->DrawIndexed(
        indexCount, 0, 0
    );
}
