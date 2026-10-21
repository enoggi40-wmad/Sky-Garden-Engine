// Rotator.cpp
#include "Rotator.h"
#include "GameObject.h"

void Rotator::Update(float deltaTime)
{
    // 자신이 붙어있는 GameObject의 Y축 회전값 증가
    gameObject->transform.rotation.y += speed * deltaTime;
}