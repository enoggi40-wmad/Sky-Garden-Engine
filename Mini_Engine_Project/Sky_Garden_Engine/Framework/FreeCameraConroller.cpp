#include "FreeCameraController.h"
#include "GameObject.h"

#include <Windows.h>
#include <DirectXMath.h>
#include "InputManager.h"

#include "CameraComponent.h"

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
    {
        return;
    }

    // 초당 이동 속도
    const float moveSpeed = 275.0f;
    float moveDistance = moveSpeed * deltaTime;

    // 현재 카메라의 회전값을 가져온다.
    XMFLOAT3 rotation = gameObject->transform.rotation;

    // Degree → Radian
    float pitch = XMConvertToRadians(rotation.x);
    float yaw = XMConvertToRadians(rotation.y);

    // 카메라 회전에 사용할 회전 행렬 생성
    XMMATRIX rotationMatrix =
        XMMatrixRotationRollPitchYaw(
            pitch,
            yaw,
            0.0f
        );

    // 기본 전방(+Z) 방향
    XMVECTOR forward =
        XMVectorSet(
            0.0f,
            0.0f,
            1.0f,
            0.0f
        );

    // 기본 오른쪽(+X) 방향
    XMVECTOR right =
        XMVectorSet(
            1.0f,
            0.0f,
            0.0f,
            0.0f
        );

    // 현재 카메라 회전을 방향 벡터에 적용
    forward =
        XMVector3TransformNormal(
            forward,
            rotationMatrix
        );

    right =
        XMVector3TransformNormal(
            right,
            rotationMatrix
        );

    // Transform의 현재 위치
    XMFLOAT3 position =
        gameObject->transform.position;

    XMVECTOR positionVector =
        XMLoadFloat3(&position);


    // W : 앞으로
    if (GetAsyncKeyState('W') & 0x8000)
    {
        positionVector += forward * moveDistance;
    }

    // S : 뒤로
    if (GetAsyncKeyState('S') & 0x8000)
    {
        positionVector -= forward * moveDistance;
    }

    // A : 왼쪽
    if (GetAsyncKeyState('A') & 0x8000)
    {
        positionVector -= right * moveDistance;
    }

    // D : 오른쪽
    if (GetAsyncKeyState('D') & 0x8000)
    {
        positionVector += right * moveDistance;
    }


    // 계산된 위치를 다시 Transform에 저장
    XMStoreFloat3(
        &gameObject->transform.position,
        positionVector
    );

   
    HWND hwnd = InputManager::GetWindowHandle();

    if (hwnd && (GetAsyncKeyState(VK_RBUTTON) & 0x8000))
    {
        const float mouseSensitivity = 0.15f;

        RECT clientRect;
        GetClientRect(hwnd, &clientRect);

        // 클라이언트 영역의 중앙 좌표
        POINT center;
        center.x = (clientRect.right - clientRect.left) / 2;
        center.y = (clientRect.bottom - clientRect.top) / 2;

        // 현재 마우스 위치
        POINT mousePosition;
        GetCursorPos(&mousePosition);

        // Screen 좌표 → Client 좌표
        ScreenToClient(hwnd, &mousePosition);

        // 화면 중앙으로부터 마우스가 얼마나 움직였는지 계산
        int deltaX = mousePosition.x - center.x;
        int deltaY = mousePosition.y - center.y;

        // 좌우 회전
        gameObject->transform.rotation.y +=
            deltaX * mouseSensitivity;

        // 상하 회전
        gameObject->transform.rotation.x +=
            deltaY * mouseSensitivity;

        // 마우스를 다시 화면 중앙으로 이동
        POINT screenCenter = center;
        ClientToScreen(hwnd, &screenCenter);

        SetCursorPos(
            screenCenter.x,
            screenCenter.y
        );
    }
}