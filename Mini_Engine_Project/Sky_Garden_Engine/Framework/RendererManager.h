#pragma once

#include <vector>

#include "Renderer.h"
#include <d3d11.h>
#include <DirectXMath.h>


class D3DClass;
class ShaderManager;

class RendererManager
{
public:
    RendererManager();
    ~RendererManager();

    void AddRenderer(Renderer* renderer);
    void Clear();

    bool Render(
        D3DClass* d3d,
        ID3D11DeviceContext* deviceContext,
        ShaderManager* shaderManager,
        DirectX::XMMATRIX viewMatrix,
        DirectX::XMMATRIX projectionMatrix,
        DirectX::XMMATRIX orthoMatrix,
        DirectX::XMFLOAT3 cameraPosition
    );

    float GetDistanceSquared(
        const DirectX::XMFLOAT3& a,
        const DirectX::XMFLOAT3& b);

private:
	// 일반 불투명 오브젝트 렌더링용 Renderer 목록
    std::vector<Renderer*> m_OpaqueRenderers;

	// 투명 오브젝트 렌더링용 Renderer 목록
    std::vector<Renderer*> m_TransparentRenderers;
};