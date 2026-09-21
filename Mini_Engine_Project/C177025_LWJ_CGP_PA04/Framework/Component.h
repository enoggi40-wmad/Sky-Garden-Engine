#pragma once

class GameObject;

// 모든 Component들의 부모 클래스
class Component
{
public:

    // 이 Component가 붙어있는 GameObject의 주소를 전달받는다.
    Component(GameObject* ownerObject)
        : gameObject(ownerObject)
    {
    }

    // 부모 클래스로 사용할 예정이므로 virtual 소멸자
    virtual ~Component() = default;

    // 초기화 함수
    virtual void Initialize()
    {
    }

    // 매 프레임 실행되는 함수
    virtual void Update(float deltaTime)
    {
    }

    // 이 Component가 붙어있는 GameObject를 반환
    GameObject* GetGameObject() const
    {
        return gameObject;
    }

protected:

    // 이 Component가 붙어있는 실제 GameObject
    GameObject* gameObject;
};