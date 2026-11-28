
#include "PhongShaderClass.h"

using namespace DirectX;
using namespace std;

// ==========================================
// Constructor / Destructor
// ==========================================

PhongShaderClass::PhongShaderClass()
{
}

PhongShaderClass::~PhongShaderClass()
{
}

// ==========================================
// Initialize / Shutdown
// ==========================================

bool PhongShaderClass::Initialize(
    ID3D11Device* device,
    HWND hwnd)
{
    return InitializeShader(
        device,
        hwnd,
        L"./data/phongShader.hlsl"
    );
}

void PhongShaderClass::Shutdown()
{
    ShutdownShader();
}

// ==========================================
// Render
// ==========================================

bool PhongShaderClass::Render(
    ID3D11DeviceContext* deviceContext,
    int indexCount,

    XMMATRIX worldMatrix,
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix,

    ID3D11ShaderResourceView* diffuseTexture,
    ID3D11ShaderResourceView* secondTexture,
    ID3D11ShaderResourceView* alphaMap,
    ID3D11ShaderResourceView* normalMap,

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
        diffuseTexture,
        secondTexture,
        alphaMap,
        normalMap,
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

// ==========================================
// Initialize Shader
// ==========================================

bool PhongShaderClass::InitializeShader(
    ID3D11Device* device,
    HWND hwnd,
    const WCHAR* fileName)
{
    HRESULT result;

    ID3D10Blob* errorMessage = nullptr;
    ID3D10Blob* vertexShaderBuffer = nullptr;
    ID3D10Blob* pixelShaderBuffer = nullptr;

    // ======================================
    // 1. Vertex Shader Compile
    // ======================================

    result = D3DCompileFromFile(
        fileName,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "LightVertexShader",
        "vs_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        &vertexShaderBuffer,
        &errorMessage
    );

    if (FAILED(result) || !vertexShaderBuffer)
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(
                errorMessage, hwnd, fileName
            );
        }
        else
        {
            MessageBox(
                hwnd,
                fileName,
                L"Vertex Shader Compile Failed",
                MB_OK
            );
        }

        return false;
    }

    if (errorMessage)
    {
        errorMessage->Release();
        errorMessage = nullptr;
    }

    // ======================================
    // 2. Pixel Shader Compile
    // ======================================

    result = D3DCompileFromFile(
        fileName,
        nullptr,
        D3D_COMPILE_STANDARD_FILE_INCLUDE,
        "LightPixelShader",
        "ps_5_0",
        D3D10_SHADER_ENABLE_STRICTNESS,
        0,
        &pixelShaderBuffer,
        &errorMessage
    );

    if (FAILED(result) || !pixelShaderBuffer)
    {
        if (errorMessage)
        {
            OutputShaderErrorMessage(
                errorMessage, hwnd, fileName
            );
        }
        else
        {
            MessageBox(
                hwnd,
                fileName,
                L"Pixel Shader Compile Failed",
                MB_OK
            );
        }

        vertexShaderBuffer->Release();

        return false;
    }

    if (errorMessage)
    {
        errorMessage->Release();
        errorMessage = nullptr;
    }

    // ======================================
    // 3. Create Vertex Shader
    // ======================================

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

    // ======================================
    // 4. Create Pixel Shader
    // ======================================

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

    // ======================================
    // 5. Input Layout
    //
    // ModelClass::VertexType:
    // Position / Texcoord / Normal /
    // Tangent / Binormal
    // ======================================

    D3D11_INPUT_ELEMENT_DESC polygonLayout[5] = {};

    // POSITION
    polygonLayout[0].SemanticName = "POSITION";
    polygonLayout[0].SemanticIndex = 0;
    polygonLayout[0].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[0].InputSlot = 0;
    polygonLayout[0].AlignedByteOffset = 0;
    polygonLayout[0].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    // TEXCOORD
    polygonLayout[1].SemanticName = "TEXCOORD";
    polygonLayout[1].SemanticIndex = 0;
    polygonLayout[1].Format =
        DXGI_FORMAT_R32G32_FLOAT;
    polygonLayout[1].InputSlot = 0;
    polygonLayout[1].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[1].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    // NORMAL
    polygonLayout[2].SemanticName = "NORMAL";
    polygonLayout[2].SemanticIndex = 0;
    polygonLayout[2].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[2].InputSlot = 0;
    polygonLayout[2].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[2].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    // TANGENT
    polygonLayout[3].SemanticName = "TANGENT";
    polygonLayout[3].SemanticIndex = 0;
    polygonLayout[3].Format =
        DXGI_FORMAT_R32G32B32_FLOAT;
    polygonLayout[3].InputSlot = 0;
    polygonLayout[3].AlignedByteOffset =
        D3D11_APPEND_ALIGNED_ELEMENT;
    polygonLayout[3].InputSlotClass =
        D3D11_INPUT_PER_VERTEX_DATA;

    // BINORMAL
    polygonLayout[4].SemanticName = "BINORMAL";
    polygonLayout[4].SemanticIndex = 0;
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

    // ======================================
    // 6. Sampler State
    // ======================================

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

    // ======================================
    // 7. Constant Buffers
    //
    // VS b0: MatrixBuffer
    // VS b1: CameraBuffer
    //
    // PS b0: LightBuffer
    // PS b1: LightDataBuffer
    // PS b2: TextureOptionBuffer
    // ======================================

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
                    &desc,
                    nullptr,
                    buffer
                )
            );
        };

    static_assert(
        sizeof(TextureOptionBufferType) % 16 == 0,
        "TextureOptionBuffer must be 16-byte aligned in size"
        );

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
            &m_lightDataBuffer) ||

        !createDynamicBuffer(
            sizeof(TextureOptionBufferType),
            &m_textureOptionBuffer))
    {
        ShutdownShader();
        return false;
    }

    return true;
}

// ==========================================
// Shutdown Shader
// ==========================================

void PhongShaderClass::ShutdownShader()
{
    if (m_textureOptionBuffer)
    {
        m_textureOptionBuffer->Release();
        m_textureOptionBuffer = nullptr;
    }

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

// ==========================================
// Shader Error
// ==========================================

void PhongShaderClass::OutputShaderErrorMessage(
    ID3D10Blob* errorMessage,
    HWND hwnd,
    const WCHAR* shaderFilename)
{
    char* compileErrors =
        static_cast<char*>(
            errorMessage->GetBufferPointer()
            );

    unsigned long bufferSize =
        static_cast<unsigned long>(
            errorMessage->GetBufferSize()
            );

    ofstream fout("shader-error.txt");

    if (fout.is_open())
    {
        for (unsigned long i = 0; i < bufferSize; ++i)
        {
            fout << compileErrors[i];
        }

        fout.close();
    }

    errorMessage->Release();

    MessageBox(
        hwnd,
        L"Error compiling shader. Check shader-error.txt.",
        shaderFilename,
        MB_OK
    );
}

// ==========================================
// Set Shader Parameters
// ==========================================

bool PhongShaderClass::SetShaderParameters(
    ID3D11DeviceContext* deviceContext,

    XMMATRIX worldMatrix,
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix,

    ID3D11ShaderResourceView* diffuseTexture,
    ID3D11ShaderResourceView* secondTexture,
    ID3D11ShaderResourceView* alphaMap,
    ID3D11ShaderResourceView* normalMap,

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

    // ======================================
    // 1. MatrixBuffer : VS b0
    //
    // 기존 행벡터 규약 유지
    // World * View * Projection
    // ======================================

    worldMatrix = XMMatrixTranspose(worldMatrix);
    viewMatrix = XMMatrixTranspose(viewMatrix);
    projectionMatrix =
        XMMatrixTranspose(projectionMatrix);

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

    // ======================================
    // 2. Texture Resources : PS t0 ~ t3
    // ======================================

    ID3D11ShaderResourceView* textures[4] =
    {
        diffuseTexture,
        secondTexture,
        alphaMap,
        normalMap
    };

    deviceContext->PSSetShaderResources(
        0,
        4,
        textures
    );

    // ======================================
    // 3. CameraBuffer : VS b1
    // ======================================

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

    // ======================================
    // 4. LightBuffer : PS b0
    // ======================================

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
    lightPtr->emissiveIntensity = emissiveIntensity;

    lightPtr->emissivePadding[0] = 0.0f;
    lightPtr->emissivePadding[1] = 0.0f;
    lightPtr->emissivePadding[2] = 0.0f;

    deviceContext->Unmap(m_lightBuffer, 0);

    deviceContext->PSSetConstantBuffers(
        0, 1, &m_lightBuffer
    );

    // ======================================
    // 5. LightDataBuffer : PS b1
    // ======================================

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

    // ======================================
    // 6. TextureOptionBuffer : PS b2
    // ======================================

    const bool useAlphaMap =
        secondTexture != nullptr &&
        alphaMap != nullptr;

    const bool useNormalMap =
        normalMap != nullptr;

    result = deviceContext->Map(
        m_textureOptionBuffer,
        0,
        D3D11_MAP_WRITE_DISCARD,
        0,
        &mappedResource
    );

    if (FAILED(result))
        return false;

    TextureOptionBufferType* optionPtr =
        static_cast<TextureOptionBufferType*>(
            mappedResource.pData
            );

    optionPtr->useAlphaMap =
        useAlphaMap ? 1 : 0;

    optionPtr->useNormalMap =
        useNormalMap ? 1 : 0;

    optionPtr->padding[0] = 0.0f;
    optionPtr->padding[1] = 0.0f;

    deviceContext->Unmap(
        m_textureOptionBuffer,
        0
    );

    deviceContext->PSSetConstantBuffers(
        2,
        1,
        &m_textureOptionBuffer
    );

    return true;
}

// ==========================================
// Render Shader
// ==========================================

void PhongShaderClass::RenderShader(
    ID3D11DeviceContext* deviceContext,
    int indexCount)
{
    deviceContext->IASetInputLayout(m_layout);

    deviceContext->VSSetShader(
        m_vertexShader,
        nullptr,
        0
    );

    deviceContext->PSSetShader(
        m_pixelShader,
        nullptr,
        0
    );

    deviceContext->PSSetSamplers(
        0,
        1,
        &m_sampleState
    );

    deviceContext->DrawIndexed(
        indexCount,
        0,
        0
    );
}
