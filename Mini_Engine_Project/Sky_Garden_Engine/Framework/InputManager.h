#pragma once

#include <Windows.h>

class InputManager
{
public:
    static void SetWindowHandle(HWND hwnd);
    static HWND GetWindowHandle();

private:
    static HWND m_hwnd;
};