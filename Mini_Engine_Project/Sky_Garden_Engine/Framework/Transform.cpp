#include "Transform.h"

// position, rotation, scale 값을 이용해
// 이 GameObject의 World Matrix를 계산한다.
XMMATRIX Transform::GetWorldMatrix() const
{
    // 크기 행렬
    XMMATRIX scaleMatrix =
        XMMatrixScaling(
            scale.x,
            scale.y,
            scale.z
        );

    // 회전 행렬
    XMMATRIX rotationMatrix =
        XMMatrixRotationRollPitchYaw(
            rotation.x,
            rotation.y,
            rotation.z
        );

    // 위치 이동 행렬
    XMMATRIX translationMatrix =
        XMMatrixTranslation(
            position.x,
            position.y,
            position.z
        );

    // Scale → Rotation → Translation 순서로 적용
    return scaleMatrix * rotationMatrix * translationMatrix;
}