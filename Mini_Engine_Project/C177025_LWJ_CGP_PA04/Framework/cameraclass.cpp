////////////////////////////////////////////////////////////////////////////////
// Filename: cameraclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "cameraclass.h"


CameraClass::CameraClass()
{
	m_position.x = 0.0f;
	m_position.y = 0.0f;
	m_position.z = 0.0f;

	m_rotation.x = 0.0f;
	m_rotation.y = 0.0f;
	m_rotation.z = 0.0f;

	m_lastMousePos.x = 0;
	m_lastMousePos.y = 0;
	m_firstMouse = true;
}


CameraClass::CameraClass(const CameraClass& other)
{
}


CameraClass::~CameraClass()
{
}


void CameraClass::SetPosition(float x, float y, float z)
{
	m_position.x = x;
	m_position.y = y;
	m_position.z = z;
}


void CameraClass::SetRotation(float x, float y, float z)
{
	m_rotation.x = x;
	m_rotation.y = y;
	m_rotation.z = z;
}


XMFLOAT3 CameraClass::GetPosition()
{
	return m_position;
}


XMFLOAT3 CameraClass::GetRotation()
{
	return m_rotation;
}

float CameraClass::GetRotationY()
{
	return m_rotation.y;
}

void CameraClass::UpdateFreeCamera(
    HWND hwnd,
    float deltaTime)
{
    const float moveSpeed = 200.0f;
    const float mouseSensitivity = 0.15f;

    float moveDistance =
        moveSpeed * deltaTime;


    // -----------------------------
    // Mouse rotation
    // Right mouse button + mouse movement
    // -----------------------------
    POINT currentMousePos;
    GetCursorPos(&currentMousePos);
    ScreenToClient(hwnd, &currentMousePos);

    RECT clientRect;
    GetClientRect(hwnd, &clientRect);

    POINT centerPos;
    centerPos.x =
        (clientRect.right - clientRect.left) / 2;

    centerPos.y =
        (clientRect.bottom - clientRect.top) / 2;


    if (GetAsyncKeyState(VK_RBUTTON) & 0x8000)
    {
        int deltaX =
            currentMousePos.x - centerPos.x;

        int deltaY =
            currentMousePos.y - centerPos.y;


        m_rotation.y +=
            deltaX * mouseSensitivity;

        m_rotation.x +=
            deltaY * mouseSensitivity;


        POINT screenCenter =
            centerPos;

        ClientToScreen(
            hwnd,
            &screenCenter
        );

        SetCursorPos(
            screenCenter.x,
            screenCenter.y
        );
    }


    // -----------------------------
    // Calculate forward / right
    // -----------------------------
    float pitch =
        m_rotation.x * 0.0174532925f;

    float yaw =
        m_rotation.y * 0.0174532925f;


    XMMATRIX rotationMatrix =
        XMMatrixRotationRollPitchYaw(
            pitch,
            yaw,
            0.0f
        );


    XMVECTOR defaultForward =
        XMVectorSet(
            0.0f,
            0.0f,
            1.0f,
            0.0f
        );

    XMVECTOR defaultRight =
        XMVectorSet(
            1.0f,
            0.0f,
            0.0f,
            0.0f
        );


    XMVECTOR forward =
        XMVector3TransformCoord(
            defaultForward,
            rotationMatrix
        );

    XMVECTOR right =
        XMVector3TransformCoord(
            defaultRight,
            rotationMatrix
        );


    forward =
        XMVector3Normalize(forward);

    right =
        XMVector3Normalize(right);


    XMVECTOR position =
        XMLoadFloat3(&m_position);


    // -----------------------------
    // Arrow / WASD movement
    // -----------------------------

    // Forward
    if ((GetAsyncKeyState(VK_UP) & 0x8000) ||
        (GetAsyncKeyState('W') & 0x8000))
    {
        position +=
            forward * moveDistance;
    }


    // Backward
    if ((GetAsyncKeyState(VK_DOWN) & 0x8000) ||
        (GetAsyncKeyState('S') & 0x8000))
    {
        position -=
            forward * moveDistance;
    }


    // Left
    if ((GetAsyncKeyState(VK_LEFT) & 0x8000) ||
        (GetAsyncKeyState('A') & 0x8000))
    {
        position -=
            right * moveDistance;
    }


    // Right
    if ((GetAsyncKeyState(VK_RIGHT) & 0x8000) ||
        (GetAsyncKeyState('D') & 0x8000))
    {
        position +=
            right * moveDistance;
    }


    XMFLOAT3 newPosition;

    XMStoreFloat3(
        &newPosition,
        position
    );


    // -----------------------------
    // Camera movement boundary
    // -----------------------------
    if (newPosition.x < -800.0f)
        newPosition.x = -800.0f;

    if (newPosition.x > 800.0f)
        newPosition.x = 800.0f;


    if (newPosition.z < -800.0f)
        newPosition.z = -800.0f;

    if (newPosition.z > 800.0f)
        newPosition.z = 800.0f;


    if (newPosition.y < -650.0f)
        newPosition.y = -650.0f;

    if (newPosition.y > 650.0f)
        newPosition.y = 650.0f;


    m_position =
        newPosition;
}

// This uses the position and rotation of the camera to build and to update the view matrix.
void CameraClass::Render()
{
	XMVECTOR up, position, lookAt;
	float yaw, pitch, roll;
	XMMATRIX rotationMatrix;

	// Setup the vector that points upwards.
	up = XMVectorSet(0.0f, 1.0f, 0.0f, 0.0f);

	// Setup the position of the camera in the world.
	position = XMLoadFloat3(&m_position);

	// Setup where the camera is looking by default.
	lookAt = XMVectorSet(0.0f, 0.0f, 1.0f, 0.0f);

	// Set the yaw (Y axis), pitch (X axis), and roll (Z axis) rotations in radians.
	pitch = m_rotation.x * 0.0174532925f;
	yaw   = m_rotation.y * 0.0174532925f;
	roll  = m_rotation.z * 0.0174532925f;

	// Create the rotation matrix from the yaw, pitch, and roll values.
	rotationMatrix = XMMatrixRotationRollPitchYaw(pitch, yaw, roll);

	// Transform the lookAt and up vector by the rotation matrix so the view is correctly rotated at the origin.
	lookAt = XMVector3TransformCoord(lookAt, rotationMatrix);
	up = XMVector3TransformCoord(up, rotationMatrix);

	// Translate the rotated camera position to the location of the viewer.
	lookAt = position + lookAt;

	// Finally create the view matrix from the three updated vectors.
	m_viewMatrix = XMMatrixLookAtLH(position, lookAt, up);

	return;
}


void CameraClass::GetViewMatrix(XMMATRIX& viewMatrix)
{
	viewMatrix = m_viewMatrix;
}