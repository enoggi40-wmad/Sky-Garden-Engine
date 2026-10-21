#include "InputManager.h"

HWND InputManager::m_hwnd = nullptr;

void InputManager::SetWindowHandle(HWND hwnd)
{
    m_hwnd = hwnd;
}

HWND InputManager::GetWindowHandle()
{
    return m_hwnd;
}