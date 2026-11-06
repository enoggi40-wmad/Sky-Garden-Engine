
#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// GameObject의 위치, 회전, 크기 정보를 관리하는 클래스
// 부모-자식 관계를 통해 씬 그래프의 변환을 처리한다.
class Transform
{
public:

    // 오브젝트의 회전값 (라디안)
    XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };

    // 오브젝트의 크기
    XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

    // 월드 행렬을 기준으로 Transform 설정
    bool SetWorldMatrix(const XMMATRIX& worldMatrix);

    // 부모 변환까지 적용한 월드 변환 행렬
    XMMATRIX GetWorldMatrix() const;

    // 부모를 기준으로 한 로컬 변환 행렬
    XMMATRIX GetLocalMatrix() const;

    // 부모 기준 위치 반환
    XMFLOAT3 GetLocalPosition() const;

    // 부모 기준 위치 설정
    void SetLocalPosition(const XMFLOAT3& localPosition);

    // 월드 위치 반환
    XMFLOAT3 GetWorldPosition() const;

    // 월드 위치 설정
    bool SetWorldPosition(const XMFLOAT3& worldPosition);

private:

    // GameObject만 부모 Transform을 변경할 수 있음
    friend class GameObject;

    // 행렬을 위치, 회전, 크기로 분해해서 저장
    bool TrySetLocalMatrix(const XMMATRIX& matrix);

    // 부모 Transform
    Transform* m_Parent = nullptr;

    // 부모 기준 로컬 위치
    XMFLOAT3 m_LocalPosition = { 0.0f, 0.0f, 0.0f };
};
