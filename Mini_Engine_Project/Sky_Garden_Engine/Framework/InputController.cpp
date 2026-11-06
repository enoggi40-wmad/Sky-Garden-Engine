
#include "InputController.h"
#include "GameObject.h"

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

    // WASD 이동
    if (GetAsyncKeyState('W') & 0x8000)
        z += 1.0f;

    if (GetAsyncKeyState('S') & 0x8000)
        z -= 1.0f;

    if (GetAsyncKeyState('A') & 0x8000)
        x -= 1.0f;

    if (GetAsyncKeyState('D') & 0x8000)
        x += 1.0f;

    // QE 상하 이동
    if (GetAsyncKeyState('Q') & 0x8000)
        y += 1.0f;

    if (GetAsyncKeyState('E') & 0x8000)
        y -= 1.0f;

    // 부모 기준 좌표 이동
    gameObject->transform.TranslateLocal({
        x * moveAmount,
        y * moveAmount,
        z * moveAmount
        });
}
