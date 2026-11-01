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
    // 처음 배치된 부모 기준 Y 위치 저장
    m_StartY =
        gameObject->transform.GetLocalPosition().y;
}

void Floater::Update(float deltaTime)
{
    // 시간 누적
    m_Time += deltaTime;

    // Sin 곡선을 이용해 상하 이동량 계산
    float offsetY =
        sinf(m_Time * m_Speed + m_StartPhase)
        * m_Amplitude;

    // 현재 로컬 위치를 가져온다.
    DirectX::XMFLOAT3 localPosition =
        gameObject->transform.GetLocalPosition();

    // X, Z는 유지하고 Y만 변경한다.
    localPosition.y = m_StartY + offsetY;

    gameObject->transform.SetLocalPosition(localPosition);
}