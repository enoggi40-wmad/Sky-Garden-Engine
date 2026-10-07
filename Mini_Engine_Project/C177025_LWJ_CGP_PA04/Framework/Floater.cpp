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
    // 처음 배치된 Y 위치 저장
    m_StartY =
        gameObject->transform.position.y;
}


void Floater::Update(float deltaTime)
{
    // 시간 누적
    m_Time += deltaTime;


    // Sin 곡선을 이용해 위아래 위치 계산
    float offsetY =
        sinf(
            m_Time * m_Speed
            + m_StartPhase
        )
        * m_Amplitude;


    // 처음 Y 위치를 기준으로 상하 이동
    gameObject->transform.position.y =
        m_StartY + offsetY;
}