#include "ModelRenderer.h"

#include "GameObject.h"
#include "modelclass.h"
#include "textureshaderclass.h"



// ModelRenderer 생성자
//
// ownerObject:
// 이 Component가 붙을 GameObject
//
// model:
// 이 GameObject가 사용할 실제 ModelClass
ModelRenderer::ModelRenderer(
    GameObject* ownerObject,
    ModelClass* model)
    : Renderer(ownerObject), m_Model(model)
{
}


// 현재 연결되어 있는 ModelClass 반환
ModelClass* ModelRenderer::GetModel() const
{
    return m_Model;
}

bool ModelRenderer::Render(
    ID3D11DeviceContext* deviceContext,
    TextureShaderClass* textureShader,
    XMMATRIX viewMatrix,
    XMMATRIX projectionMatrix,
    XMFLOAT3 cameraPosition)
{
    if (!m_Model)
    {
        // ModelClass가 연결되어 있지 않으면 렌더링할 수 없음
        return false;
	}

    // 이 ModelRenderer가 붙어있는 GameObject의
    // Transform을 이용해서 World Matrix를 만든다.
    //
    // position / rotation / scale 값이 모두 여기 반영된다.
    XMMATRIX worldMatrix =
        gameObject->transform.GetWorldMatrix();


    // ModelClass가 가지고 있는 Vertex / Index Buffer를
    // Direct3D 렌더링 파이프라인에 준비한다.
    m_Model->Render(deviceContext);


    // Texture Shader를 이용해서 실제 모델을 화면에 그린다.
    //
    // 기존에는 GraphicsClass에서 직접 하던 작업이었지만,
    // 이제 ModelRenderer가 담당하게 된다.
    bool result = textureShader->Render(
        deviceContext,
        m_Model->GetIndexCount(),
        worldMatrix,
        viewMatrix,
        projectionMatrix,
        m_Model->GetTexture()
    );


    // Shader 렌더링에 실패한 경우
    if (!result)
    {
        return false;
    }


    // 정상적으로 렌더링 완료
    return true;
}