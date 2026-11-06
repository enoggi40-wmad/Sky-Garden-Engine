#include "InputController.h"
#include "GameObject.h"

#include <Windows.h>
#include <DirectXMath.h>

using namespace DirectX;

InputController::InputController(
    GameObject* ownerObject,
    float moveSpeed)
    : Component(ownerObject),
    m_MoveSpeed(moveSpeed)
{
}

void InputController::Update(float deltaTime)
{
    XMFLOAT3 position =
        gameObject->transform.GetLocalPosition();

    float moveAmount = m_MoveSpeed * deltaTime;

    // WASD 이동
    if (GetAsyncKeyState('W') & 0x8000)
        position.z += moveAmount;

    if (GetAsyncKeyState('S') & 0x8000)
        position.z -= moveAmount;

    if (GetAsyncKeyState('A') & 0x8000)
        position.x -= moveAmount;

    if (GetAsyncKeyState('D') & 0x8000)
        position.x += moveAmount;

    // QE 상하 이동
    if (GetAsyncKeyState('Q') & 0x8000)
        position.y += moveAmount;

    if (GetAsyncKeyState('E') & 0x8000)
        position.y -= moveAmount;

    gameObject->transform.SetLocalPosition(position);
}