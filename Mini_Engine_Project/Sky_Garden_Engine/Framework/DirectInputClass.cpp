#include "DirectInputClass.h"
#include <cstring>

#pragma comment(lib, "dinput8.lib")
#pragma comment(lib, "dxguid.lib")

DirectInputClass& DirectInputClass::Get()
{
    static DirectInputClass instance;
    return instance;
}

DirectInputClass::~DirectInputClass()
{
    Shutdown();
}

bool DirectInputClass::Initialize(HINSTANCE hInstance, HWND hwnd)
{
    Shutdown();

    if (FAILED(DirectInput8Create(hInstance, DIRECTINPUT_VERSION,
        IID_IDirectInput8, reinterpret_cast<void**>(&m_DirectInput), nullptr)))
        return false;

    if (FAILED(m_DirectInput->CreateDevice(GUID_SysKeyboard, &m_Keyboard, nullptr)) ||
        FAILED(m_Keyboard->SetDataFormat(&c_dfDIKeyboard)) ||
        FAILED(m_Keyboard->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
    {
        Shutdown();
        return false;
    }

    if (FAILED(m_DirectInput->CreateDevice(GUID_SysMouse, &m_Mouse, nullptr)) ||
        FAILED(m_Mouse->SetDataFormat(&c_dfDIMouse)) ||
        FAILED(m_Mouse->SetCooperativeLevel(hwnd, DISCL_FOREGROUND | DISCL_NONEXCLUSIVE)))
    {
        Shutdown();
        return false;
    }

    m_Keyboard->Acquire();
    m_Mouse->Acquire();
    return true;
}

void DirectInputClass::Shutdown()
{
    if (m_Mouse) { m_Mouse->Unacquire(); m_Mouse->Release(); m_Mouse = nullptr; }
    if (m_Keyboard) { m_Keyboard->Unacquire(); m_Keyboard->Release(); m_Keyboard = nullptr; }
    if (m_DirectInput) { m_DirectInput->Release(); m_DirectInput = nullptr; }
    std::memset(m_KeyboardState, 0, sizeof(m_KeyboardState));
    m_MouseState = {};
}

void DirectInputClass::Update()
{
    std::memset(m_KeyboardState, 0, sizeof(m_KeyboardState));
    m_MouseState = {};

    if (m_Keyboard)
    {
        HRESULT result = m_Keyboard->GetDeviceState(sizeof(m_KeyboardState), m_KeyboardState);
        if (FAILED(result))
        {
            m_Keyboard->Acquire();
            std::memset(m_KeyboardState, 0, sizeof(m_KeyboardState));
        }
    }

    if (m_Mouse)
    {
        HRESULT result = m_Mouse->GetDeviceState(sizeof(m_MouseState), &m_MouseState);
        if (FAILED(result))
        {
            m_Mouse->Acquire();
            m_MouseState = {};
        }
    }
}

bool DirectInputClass::IsKeyDown(unsigned int key) const
{
    return key < 256 && (m_KeyboardState[key] & 0x80) != 0;
}

bool DirectInputClass::IsMouseButtonDown(unsigned int button) const
{
    return button < 4 && (m_MouseState.rgbButtons[button] & 0x80) != 0;
}

long DirectInputClass::GetMouseDeltaX() const { return m_MouseState.lX; }
long DirectInputClass::GetMouseDeltaY() const { return m_MouseState.lY; }
