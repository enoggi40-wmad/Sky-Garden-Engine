
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
    // 기존 Transform의 회전은 기본값(회전 없음)을 가정
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

    // 현재 Transform에 저장된 Quaternion 가져오기
    XMFLOAT4 rotation =
        gameObject->transform.GetLocalRotation();

    // Quaternion으로 회전 행렬 생성
    XMMATRIX rotationMatrix =
        XMMatrixRotationQuaternion(
            XMLoadFloat4(&rotation)
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

    // 현재 카메라 회전을 이동 방향에 적용
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

    // 현재 카메라 위치 가져오기
    XMFLOAT3 position =
        gameObject->transform.GetLocalPosition();

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

    // 계산한 위치 저장
    XMFLOAT3 newPosition;
    XMStoreFloat3(&newPosition, positionVector);

    gameObject->transform.SetLocalPosition(newPosition);

    // 마우스 우클릭 시 카메라 회전
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

        ScreenToClient(hwnd, &mousePosition);

        // 화면 중앙으로부터 마우스 이동량
        int deltaX = mousePosition.x - center.x;
        int deltaY = mousePosition.y - center.y;

        // 기존처럼 Degree 단위로 회전각 누적
        m_Yaw += deltaX * mouseSensitivity;
        m_Pitch += deltaY * mouseSensitivity;

        // Euler Angle을 Quaternion으로 변환
        gameObject->transform.SetLocalEulerAngles({ m_Pitch,m_Yaw,0.0f });

        // 마우스를 화면 중앙으로 이동
        POINT screenCenter = center;
        ClientToScreen(hwnd, &screenCenter);

        SetCursorPos(
            screenCenter.x,
            screenCenter.y
        );
    }
}
