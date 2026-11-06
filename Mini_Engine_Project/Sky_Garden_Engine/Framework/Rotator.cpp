
#include "Rotator.h"
#include "GameObject.h"

void Rotator::Update(float deltaTime)
{
    // 로컬 Y축으로 지속 회전
    gameObject->transform.Rotate({
        0.0f,
        speed * deltaTime,
        0.0f
        });
}
