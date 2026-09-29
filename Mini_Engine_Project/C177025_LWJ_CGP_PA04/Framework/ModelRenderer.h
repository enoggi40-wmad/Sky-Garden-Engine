#pragma once

#include "Component.h"
#include <DirectXMath.h>

using namespace DirectX;

class TextureShaderClass;

struct ID3D11DeviceContext;

// ModelClass를 포인터로만 사용할 것이기 때문에
// 여기서는 전체 내용을 include하지 않고 전방 선언만 한다.
class ModelClass;


// GameObject에 "3D 모델을 그리는 기능"을 추가하는 Component
//
// 예를 들어 GuardianLion GameObject에 이 Component를 붙이면
// 해당 GameObject가 어떤 ModelClass를 사용할지 연결할 수 있다.
//
// 나중에는 GameObject의 Transform을 읽어서
// 실제 World Matrix를 만들고 렌더링하는 역할까지 담당하게 된다.
class ModelRenderer : public Component
{
public:

    // 생성자
    //
    // ownerObject:
    // 이 ModelRenderer가 붙어있는 GameObject의 주소
    //
    // model:
    // 실제로 화면에 그릴 ModelClass의 주소
    //
    // 예:
    // GuardianLion GameObject + GuardianLion ModelClass
    // 를 서로 연결할 때 사용한다.
    ModelRenderer(GameObject* ownerObject, ModelClass* model);


    // 소멸자
    //
    // ModelRenderer 객체가 삭제될 때 호출된다.
    //
    // virtual:
    // Component* 같은 부모 클래스 포인터로 삭제하더라도
    // 실제 자식 클래스인 ModelRenderer의 소멸자가
    // 정상적으로 호출되도록 한다.
    //
    // = default:
    // 소멸할 때 특별히 직접 처리할 내용이 없으므로
    // C++이 제공하는 기본 소멸자를 사용한다.
    virtual ~ModelRenderer() = default;


    // 현재 ModelRenderer에 연결되어 있는
    // ModelClass의 주소를 반환하는 Getter 함수
    //
    // m_Model은 private이기 때문에
    // 외부에서 직접 접근하지 않고 이 함수를 통해 가져온다.
    //
    // 뒤의 const는
    // 이 함수가 ModelRenderer 자신의 멤버 값을
    // 변경하지 않는다는 의미이다.
    ModelClass* GetModel() const;

    // 이 GameObject에 연결된 모델을 실제로 렌더링한다.
    //
    // 렌더링에 성공하면 true,
    // 실패하면 false를 반환한다.
    bool Render(
        ID3D11DeviceContext* deviceContext,
        TextureShaderClass* textureShader,
        XMMATRIX viewMatrix,
        XMMATRIX projectionMatrix
    );


private:

    // 실제 모델 데이터를 가지고 있는 ModelClass의 주소
    ModelClass* m_Model;
};