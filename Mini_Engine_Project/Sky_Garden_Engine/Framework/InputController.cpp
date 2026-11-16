
#include "InputController.h"
#include "GameObject.h"
#include "DirectInputClass.h"

#include <Windows.h>

InputController::InputController(
    GameObject* ownerObject,
    float moveSpeed)
    : Component(ownerObject),
    m_MoveSpeed(moveSpeed)
{
}

void InputController::Update(float deltaTime)
{
    float moveAmount = m_MoveSpeed * deltaTime;

    // 입력 방향
    float x = 0.0f;
    float y = 0.0f;
    float z = 0.0f;

    auto& input = DirectInputClass::Get();

    // wasd
    if (input.IsKeyDown(DIK_W)) z += 1.0f;
    if (input.IsKeyDown(DIK_S)) z -= 1.0f;
    if (input.IsKeyDown(DIK_A)) x -= 1.0f;
    if (input.IsKeyDown(DIK_D)) x += 1.0f;
    // qe
    if (input.IsKeyDown(DIK_Q)) y += 1.0f;
    if (input.IsKeyDown(DIK_E)) y -= 1.0f;

    // 부모 기준 좌표 이동
    gameObject->transform.TranslateLocal({
        x * moveAmount,
        y * moveAmount,
        z * moveAmount
        });
}
