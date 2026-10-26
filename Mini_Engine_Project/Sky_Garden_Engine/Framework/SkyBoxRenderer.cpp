#include "SkyBoxRenderer.h"

#include "SkySphereClass.h"
#include "ShaderManager.h"
#include "SkyMapShaderClass.h"

#include <DirectXMath.h>

using namespace DirectX;


SkyBoxRenderer::SkyBoxRenderer(
    GameObject* owner,
    ID3D11ShaderResourceView* cubeMapTexture)
    : Renderer(owner)
{
    m_SkySphere = nullptr;
    m_CubeMapTexture = cubeMapTexture;

    m_RasterState = nullptr;
    m_DepthStencilState = nullptr;
}


SkyBoxRenderer::~SkyBoxRenderer()
{
    if (m_SkySphere)
    {
        m_SkySphere->Shutdown();

        delete m_SkySphere;
        m_SkySphere = nullptr;
    }


    if (m_RasterState)
    {
        m_RasterState->Release();
        m_RasterState = nullptr;
    }


    if (m_DepthStencilState)
    {
        m_DepthStencilState->Release();
        m_DepthStencilState = nullptr;
    }


    // m_CubeMapTexture는 ResourceManager가 소유
    // 여기서는 Release 하지 않는다.
    m_CubeMapTexture = nullptr;
}


bool SkyBoxRenderer::Initialize(
    ID3D11Device* device)
{
    // =====================================================
    // Sky Sphere 생성
    // =====================================================

    m_SkySphere =
        new SkySphereClass;

    if (!m_SkySphere)
    {
        return false;
    }


    bool result =
        m_SkySphere->Initialize(
            device,
            10,
            10
        );

    if (!result)
    {
        return false;
    }


    // =====================================================
    // Sky 전용 Rasterizer State
    // Sphere 내부에서도 보이도록 Cull 하지 않음
    // =====================================================

    D3D11_RASTERIZER_DESC rasterDesc = {};

    rasterDesc.FillMode =
        D3D11_FILL_SOLID;

    rasterDesc.CullMode =
        D3D11_CULL_NONE;

    rasterDesc.FrontCounterClockwise =
        false;

    rasterDesc.DepthClipEnable =
        true;


    HRESULT hr =
        device->CreateRasterizerState(
            &rasterDesc,
            &m_RasterState
        );

    if (FAILED(hr))
    {
        return false;
    }


    // =====================================================
    // Sky 전용 Depth State
    // HLSL의 .xyww와 함께 사용
    // Sky를 가장 먼 깊이에 배치
    // =====================================================

    D3D11_DEPTH_STENCIL_DESC depthDesc = {};

    depthDesc.DepthEnable =
        true;

    depthDesc.DepthWriteMask =
        D3D11_DEPTH_WRITE_MASK_ALL;

    depthDesc.DepthFunc =
        D3D11_COMPARISON_LESS_EQUAL;

    depthDesc.StencilEnable =
        false;


    hr =
        device->CreateDepthStencilState(
            &depthDesc,
            &m_DepthStencilState
        );

    if (FAILED(hr))
    {
        return false;
    }


    return true;
}


bool SkyBoxRenderer::Render(
    const RenderContext& context)
{
    
    if (!m_SkySphere ||
        !m_CubeMapTexture ||
        !context.shaderManager)
    {
        return false;
    }


    SkyMapShaderClass* skyShader =
        context.shaderManager->GetSkyMapShader();

    if (!skyShader)
    {
        return false;
    }


    // =====================================================
    // 기존 Render State 저장
    // =====================================================

    ID3D11RasterizerState* oldRasterState =
        nullptr;

    ID3D11DepthStencilState* oldDepthState =
        nullptr;

    UINT oldStencilRef = 0;


    context.deviceContext->RSGetState(
        &oldRasterState
    );

    context.deviceContext->OMGetDepthStencilState(
        &oldDepthState,
        &oldStencilRef
    );


    // =====================================================
    // Sky 전용 Render State 적용
    // =====================================================

    context.deviceContext->RSSetState(
        m_RasterState
    );

    context.deviceContext->OMSetDepthStencilState(
        m_DepthStencilState,
        0
    );


    // =====================================================
    // Sky는 항상 카메라 중심에 위치
    // =====================================================

    XMMATRIX scaleMatrix =
        XMMatrixScaling(
            5.0f,
            5.0f,
            5.0f
        );


    XMMATRIX translationMatrix =
        XMMatrixTranslation(
            context.cameraPosition.x,
            context.cameraPosition.y,
            context.cameraPosition.z
        );


    XMMATRIX worldMatrix =
        scaleMatrix *
        translationMatrix;


    // =====================================================
    // Sphere Buffer 설정
    // =====================================================

    m_SkySphere->Render(
        context.deviceContext
    );


    // =====================================================
    // SkyMap Shader로 렌더링
    // =====================================================

    bool result =
        skyShader->Render(
            context.deviceContext,
            m_SkySphere->GetIndexCount(),
            worldMatrix,
            context.viewMatrix,
            context.projectionMatrix,
            m_CubeMapTexture
        );


    // =====================================================
    // 기존 Render State 복구
    // =====================================================

    context.deviceContext->RSSetState(
        oldRasterState
    );

    context.deviceContext->OMSetDepthStencilState(
        oldDepthState,
        oldStencilRef
    );


    if (oldRasterState)
    {
        oldRasterState->Release();
    }

    if (oldDepthState)
    {
        oldDepthState->Release();
    }


    return result;
}