
#include "FreeCameraController.h"
#include "GameObject.h"
#include "CameraComponent.h"
#include "InputManager.h"
#include "DirectInputClass.h"
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

    auto& input = DirectInputClass::Get();

    Transform& transform = gameObject->transform;

    // 이동 속도
    const float moveSpeed = 275.0f;
    float moveDistance = moveSpeed * deltaTime;

    // 입력 방향
    float forwardInput = 0.0f;
    float rightInput = 0.0f;

    // 방향키로 카메라 이동
    if (input.IsKeyDown(DIK_UP)) forwardInput += 1.0f;
    if (input.IsKeyDown(DIK_DOWN)) forwardInput -= 1.0f;
    if (input.IsKeyDown(DIK_RIGHT)) rightInput += 1.0f;
    if (input.IsKeyDown(DIK_LEFT)) rightInput -= 1.0f;

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
    if (input.IsMouseButtonDown(1))
    {
        const float mouseSensitivity = 0.15f;

        float deltaX = static_cast<float>(input.GetMouseDeltaX());
        float deltaY = static_cast<float>(input.GetMouseDeltaY());

        m_Yaw += deltaX * mouseSensitivity;
        m_Pitch += deltaY * mouseSensitivity;

        m_Pitch = std::clamp(
            m_Pitch,
            -89.0f,
            89.0f
        );

        transform.SetLocalEulerAngles({
            m_Pitch,
            m_Yaw,
            0.0f
            });
    }
}
