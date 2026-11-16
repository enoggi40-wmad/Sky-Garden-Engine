#pragma once

#define DIRECTINPUT_VERSION 0x0800
#include <dinput.h>

class DirectInputClass
{
public:
    static DirectInputClass& Get();

    bool Initialize(HINSTANCE hInstance, HWND hwnd);
    void Shutdown();
    void Update();

    bool IsKeyDown(unsigned int key) const;
    bool IsMouseButtonDown(unsigned int button) const;
    long GetMouseDeltaX() const;
    long GetMouseDeltaY() const;

private:
    DirectInputClass() = default;
    ~DirectInputClass();
    DirectInputClass(const DirectInputClass&) = delete;
    DirectInputClass& operator=(const DirectInputClass&) = delete;

    IDirectInput8* m_DirectInput = nullptr;
    IDirectInputDevice8* m_Keyboard = nullptr;
    IDirectInputDevice8* m_Mouse = nullptr;
    BYTE m_KeyboardState[256] = {};
    DIMOUSESTATE m_MouseState = {};
};
