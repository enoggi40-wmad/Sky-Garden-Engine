#pragma once

#include "Renderer.h"

#include <d3d11.h>

class SkySphereClass;


class SkyBoxRenderer : public Renderer
{
public:
    SkyBoxRenderer(
        GameObject* owner,
        ID3D11ShaderResourceView* cubeMapTexture
    );

    virtual ~SkyBoxRenderer();


    bool Initialize(
        ID3D11Device* device
    );


    bool Render(
        const RenderContext& context
    ) override;


private:
    // SkyBox 전용 Sphere Geometry
    SkySphereClass* m_SkySphere;

    // ResourceManager가 소유
    // SkyBoxRenderer는 참조만 함
    ID3D11ShaderResourceView* m_CubeMapTexture;


    // Sky 전용 렌더링 상태
    ID3D11RasterizerState* m_RasterState;
    ID3D11DepthStencilState* m_DepthStencilState;
};