#include "Orbiter.h"
#include "GameObject.h"

#include <cmath>


Orbiter::Orbiter(
    GameObject* ownerObject,
    DirectX::XMFLOAT3 center,
    float radius,
    float orbitSpeed,
    float startAngle)
    : Component(ownerObject),
    m_Center(center),
    m_Radius(radius),
    m_OrbitSpeed(orbitSpeed),
    m_Angle(startAngle)
{
    // GameObject가 처음 가지고 있던 Y 위치를 저장
    m_Height = gameObject->transform.position.y;
}


void Orbiter::Update(float deltaTime)
{
    // 시간에 따라 공전 각도 증가
    m_Angle += m_OrbitSpeed * deltaTime;


    // 원형 궤도상의 X, Z 위치 계산
    float x =
        m_Center.x +
        cosf(m_Angle) * m_Radius;

    float z =
        m_Center.z +
        sinf(m_Angle) * m_Radius;


    // 계산된 위치를 Transform에 적용
    gameObject->transform.position.x = x;
    gameObject->transform.position.z = z;

    // Y축 위치는 기존 높이 유지
    gameObject->transform.position.y = m_Height;
}