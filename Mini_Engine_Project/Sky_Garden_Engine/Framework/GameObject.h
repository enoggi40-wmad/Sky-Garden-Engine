#pragma once

#include <string>
#include <vector>

#include "Transform.h"
#include "Component.h"

// 게임 월드에 존재하는 하나의 객체
class GameObject
{
public:
     GameObject(const std::string& objectName);
    ~GameObject();

    // GameObject 초기화
    void Initialize();

    // 매 프레임 호출
    void Update(float deltaTime);

    // 기존 AddComponent 방식
    //void AddComponent(Component* component);

    // GameObject에 Component를 추가
    // 템플릿 함수
    // 가변 인자 템플릿
    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        T* component =
            new T(this, std::forward<Args>(args)...);

        components.push_back(component);

        return component;
    }


    // 이름 반환
    std::string GetName() const;

    // 원하는 타입의 Component를 찾아서 반환
    template<typename T>
    T* GetComponent()
    {
        // GameObject가 가지고 있는 모든 Component를 확인
        for (Component* component : components)
        {
            // 현재 Component가 T 타입인지 검사
            T* targetComponent =
                dynamic_cast<T*>(component);

            // 원하는 타입이면 반환
            if (targetComponent)
            {
                return targetComponent;
            }
        }

        // 찾지 못했으면 nullptr 반환
        return nullptr;
    }

public:
    // 모든 GameObject가 기본으로 가지는 Transform
    Transform transform;

private:
    // GameObject 이름
    std::string name;

    // 이 GameObject에 붙어있는 Component 목록
    std::vector<Component*> components;
};