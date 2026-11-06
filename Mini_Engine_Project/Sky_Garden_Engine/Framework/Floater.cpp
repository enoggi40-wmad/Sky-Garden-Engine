
#include "Floater.h"
#include "GameObject.h"

#include <cmath>

Floater::Floater(
    GameObject* ownerObject,
    float amplitude,
    float speed,
    float startPhase)
    : Component(ownerObject),
    m_Amplitude(amplitude),
    m_Speed(speed),
    m_Time(0.0f),
    m_StartPhase(startPhase)
{
    // 초기 높이 저장
    m_StartY = gameObject->transform.GetLocalPosition().y;
}

void Floater::Update(float deltaTime)
{
    // 시간 누적
    m_Time += deltaTime;

    // 상하 이동량 계산
    float offsetY =
        sinf(m_Time * m_Speed + m_StartPhase)
        * m_Amplitude;

    // 현재 위치 가져오기
    auto position = gameObject->transform.GetLocalPosition();

    // Y축 위치 변경
    position.y = m_StartY + offsetY;

    // Transform에 적용
    gameObject->transform.SetLocalPosition(position);
}
