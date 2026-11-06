
#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// GameObject의 위치, 회전, 크기를 관리하는 클래스
// 부모-자식 관계를 통해 Scene Graph 변환을 처리한다.
class Transform
{
public:

    // 오브젝트의 로컬 크기
    XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };

    // 월드 행렬 조회 및 설정
    XMMATRIX GetWorldMatrix() const;
    bool SetWorldMatrix(const XMMATRIX& worldMatrix);

    // 로컬 행렬 생성
    XMMATRIX GetLocalMatrix() const;

    // 월드 위치 조회 및 설정
    XMFLOAT3 GetWorldPosition() const;
    bool SetWorldPosition(const XMFLOAT3& worldPosition);

    // 로컬 위치 조회 및 설정
    XMFLOAT3 GetLocalPosition() const;
    void SetLocalPosition(const XMFLOAT3& localPosition);

    // 월드 회전 조회 및 설정 (Quaternion)
    XMFLOAT4 GetWorldRotation() const;
    void SetWorldRotation(const XMFLOAT4& rotation);

    // 로컬 회전 조회 및 설정 
    XMFLOAT4 GetLocalRotation() const;
    void SetLocalRotation(const XMFLOAT4& rotation);

    // 로컬 크기 조회 및 설정
    XMFLOAT3 GetLocalScale() const;
    void SetLocalScale(const XMFLOAT3& localScale);

    // 월드 크기 조회 및 설정
    bool GetWorldScale(XMFLOAT3& outScale) const;
    bool SetWorldScale(const XMFLOAT3& worldScale);

    // 오일러 각도로 로컬 회전 설정 (Degree)
    void SetLocalEulerAngles(const XMFLOAT3& eulerDegrees);

    // 로컬 회전을 오일러 각도로 반환 (Degree)
    XMFLOAT3 GetLocalEulerAngles() const;

    // 월드 기준 방향 벡터
    XMFLOAT3 Forward() const;
    XMFLOAT3 Right() const;

    // Local / World 좌표 기준 이동
    void TranslateLocal(const XMFLOAT3& movement);
    bool TranslateWorld(const XMFLOAT3& movement);

    // 현재 Transform을 기준으로 View Matrix 생성
    XMMATRIX GetViewMatrix() const;

public:
    // 로컬 좌표축을 기준으로 회전 (라디안)
    void Rotate(const XMFLOAT3& eulerRadians);

    // 월드 좌표의 중심과 회전축을 기준으로 회전
    bool RotateAround(
        const DirectX::XMFLOAT3& worldCenter,
        const DirectX::XMFLOAT3& axis,
        float angleRadians
    );

    

private:

    // GameObject만 부모 Transform 연결을 변경할 수 있음
    friend class GameObject;

    // 행렬을 Position, Rotation, Scale로 분해하여 저장
    bool TrySetLocalMatrix(const XMMATRIX& matrix);

    // 부모 Transform
    Transform* m_Parent = nullptr;

    // 부모 기준 로컬 위치
    XMFLOAT3 m_LocalPosition = {
        0.0f, 0.0f, 0.0f
    };

    // 부모 기준 로컬 회전 (Quaternion)
    // (0, 0, 0, 1)은 회전하지 않은 상태
    XMFLOAT4 m_LocalRotation = {
        0.0f, 0.0f, 0.0f, 1.0f
    };

    // 부모 기준 로컬 크기
    XMFLOAT3 m_LocalScale = {
        1.0f, 1.0f, 1.0f
    };
};
