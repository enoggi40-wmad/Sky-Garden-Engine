#include "Component.h"

// 생성자
Component::Component(GameObject* ownerObject)
    : gameObject(ownerObject)
{

}

// 소멸자
Component::~Component()
{

}

// 초기화 함수
void Component::Initialize()
{
}

// 매 프레임 실행되는 함수
void Component::Update(float deltaTime)
{

}

// 이 Component가 붙어있는 GameObject 반환
GameObject* Component::GetGameObject() const
{
    return gameObject;
}