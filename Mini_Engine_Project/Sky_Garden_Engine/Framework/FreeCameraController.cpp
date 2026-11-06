
#include "FreeCameraController.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "InputManager.h"

#include <Windows.h>
#include <algorithm>

using namespace DirectX;

FreeCameraController::FreeCameraController(GameObject* owner)
    : Component(owner)
{
}

FreeCameraController::~FreeCameraController()
{
}

void FreeCameraController::Update(float deltaTime)
{
    CameraComponent* camera =
        gameObject->GetComponent<CameraComponent>();

    if (!camera || !camera->IsActive())
        return;

    Transform& transform = gameObject->transform;

    // 이동 속도
    const float moveSpeed = 275.0f;
    float moveDistance = moveSpeed * deltaTime;

    // 입력 방향
    float forwardInput = 0.0f;
    float rightInput = 0.0f;

    if (GetAsyncKeyState('W') & 0x8000)
        forwardInput += 1.0f;

    if (GetAsyncKeyState('S') & 0x8000)
        forwardInput -= 1.0f;

    if (GetAsyncKeyState('D') & 0x8000)
        rightInput += 1.0f;

    if (GetAsyncKeyState('A') & 0x8000)
        rightInput -= 1.0f;

    // 카메라의 월드 방향 가져오기
    XMFLOAT3 forward = transform.Forward();
    XMFLOAT3 right = transform.Right();

    // 월드 기준 이동량 계산
    XMFLOAT3 movement = {
        (forward.x * forwardInput +
         right.x * rightInput) * moveDistance,

        (forward.y * forwardInput +
         right.y * rightInput) * moveDistance,

        (forward.z * forwardInput +
         right.z * rightInput) * moveDistance
    };

    // 카메라 이동
    transform.TranslateWorld(movement);

    // 마우스 우클릭 시 회전
    HWND hwnd = InputManager::GetWindowHandle();

    if (hwnd && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
    {
        const float mouseSensitivity = 0.15f;

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // 화면 중앙
        POINT center;
        center.x =
            (clientRect.right - clientRect.left) / 2;

        center.y =
            (clientRect.bottom - clientRect.top) / 2;

        // 현재 마우스 위치
        POINT mousePosition;
        GetCursorPos(&mousePosition);
        ScreenToClient(hwnd, &mousePosition);

        // 마우스 이동량
        int deltaX = mousePosition.x - center.x;
        int deltaY = mousePosition.y - center.y;

        // 회전 각도 갱신 (Degree)
        m_Yaw += deltaX * mouseSensitivity;
        m_Pitch += deltaY * mouseSensitivity;

        // 상하 회전 제한
        m_Pitch = std::clamp(
            m_Pitch,
            -89.0f,
            89.0f
        );

        // 회전 처리는 Transform에 위임
        transform.SetLocalEulerAngles({
            m_Pitch,
            m_Yaw,
            0.0f
            });

        // 마우스를 중앙으로 이동
        POINT screenCenter = center;
        ClientToScreen(hwnd, &screenCenter);

        SetCursorPos(
            screenCenter.x,
            screenCenter.y
        );
    }
}
