#include "RendererManager.h"
#include "Renderer.h"
#include "D3DClass.h"
#include <algorithm>
#include "RenderContext.h"

RendererManager::RendererManager()
{
}

RendererManager::~RendererManager()
{
}

void RendererManager::AddRenderer(Renderer* renderer)
{
    if (!renderer)
    {
        return;
    }

    if (renderer->IsTransparent())
    {
        m_TransparentRenderers.push_back(renderer);
    }
    else
    {
        m_OpaqueRenderers.push_back(renderer);
    }
}

void RendererManager::Clear()
{
    m_OpaqueRenderers.clear();
    m_TransparentRenderers.clear();
}

float RendererManager::GetDistanceSquared(
    const DirectX::XMFLOAT3& a,
    const DirectX::XMFLOAT3& b)
{
    float dx = a.x - b.x;
    float dy = a.y - b.y;
    float dz = a.z - b.z;

    return
        dx * dx +
        dy * dy +
        dz * dz;
}

bool RendererManager::Render(
    D3DClass* d3d,
    ID3D11DeviceContext* deviceContext,
    TextureShaderClass* textureShader,
    DirectX::XMMATRIX viewMatrix,
    DirectX::XMMATRIX projectionMatrix,
    DirectX::XMMATRIX orthoMatrix,
    DirectX::XMFLOAT3 cameraPosition)
{
    RenderContext context{};

    context.deviceContext = deviceContext;
    context.textureShader = textureShader;

    context.viewMatrix = viewMatrix;
    context.projectionMatrix = projectionMatrix;

    context.cameraPosition = cameraPosition;

	context.orthoMatrix = orthoMatrix;

    // 불투명 오브젝트 렌더링
    for (Renderer* renderer : m_OpaqueRenderers)
    {
        if (!renderer)
        {
            continue;
        }

        bool result = renderer->Render(context);

        if (!result)
        {
            continue;
        }
    }

    std::sort(
        m_TransparentRenderers.begin(),
        m_TransparentRenderers.end(),

        [this,cameraPosition](Renderer* a, Renderer* b)
        {
            float distanceA =
                GetDistanceSquared(
                    a->GetPosition(),
                    cameraPosition
                );

            float distanceB =
                GetDistanceSquared(
                    b->GetPosition(),
                    cameraPosition
                );

            // 먼 오브젝트가 앞으로 오도록 정렬
            return distanceA > distanceB;
        }
    );

    // 투명 오브젝트 렌더링
    d3d->TurnOnAlphaBlending();

    for (Renderer* renderer : m_TransparentRenderers)
    {
        if (!renderer)
        {
            continue;
        }

        bool result = renderer->Render(context);
            

        if (!result)
        {
            continue;
        }
    }

    // 투명 렌더링 종료
    d3d->TurnOffAlphaBlending();

    return true;
}