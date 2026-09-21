#pragma once

#include <DirectXMath.h>

using namespace DirectX;

// GameObject의 위치, 회전, 크기 정보를 관리하는 클래스
// Unity의 Transform과 비슷한 역할을 한다.
class Transform
{
public:

    // 오브젝트의 위치
    // 기본값은 월드 원점 (0, 0, 0)
    XMFLOAT3 position = { 0.0f, 0.0f, 0.0f };

    // 오브젝트의 회전값
    // x, y, z 축 기준 회전값을 저장한다.
    // 추후 World Matrix를 만들 때 사용한다.
    XMFLOAT3 rotation = { 0.0f, 0.0f, 0.0f };

    // 오브젝트의 크기
    // 기본값은 원래 크기인 (1, 1, 1)
    XMFLOAT3 scale = { 1.0f, 1.0f, 1.0f };
};