#pragma once

#include <string>
#include <vector>
#include <utility>

#include "Transform.h"
#include "Component.h"

// 템플릿 함수에서 컴포넌트 잘못 붙일때 static_assert로 컴파일 타임에 잡기 위해 필요
#include <type_traits>

class Scene;

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

            
    // GameObject에 Component를 추가

    template<typename T, typename... Args>
    T* AddComponent(Args&&... args)
    {
        static_assert(std::is_base_of_v<Component, T>,
            "T must inherit from Component");

        T* component =
            new T(this, std::forward<Args>(args)...);

        components.push_back(component);

        NotifyComponentAdded(component);

        return component;
    }



    // 이름 반환
    std::string GetName() const;

    // 원하는 타입의 Component를 찾아서 반환
    template<typename T>
    T* GetComponent()
    {
		// T가 Component를 상속받는지 확인 
        // T가 Component를 상속받지 않으면 오류메세지 뜸
        static_assert(std::is_base_of_v<Component, T>,
            "T must inherit from Component");

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

	// 부모 GameObject 반환
    GameObject* GetParent() const;
    

    // 부모 설정. 연결할 수 없으면 false 반환
    bool SetParent(
        GameObject* newParent,
        bool keepWorldTransform = true
    );

public:
    // 모든 GameObject가 기본으로 가지는 Transform
    Transform transform;

private:
    // GameObject 이름
    std::string name;

    // 이 GameObject에 붙어있는 Component 목록
    std::vector<Component*> components;

	// 부모 GameObject
	GameObject* m_Parent = nullptr;

	// 자식 GameObject 목록
	std::vector<GameObject*> m_Children;

    friend class Scene;

    Scene* m_Scene = nullptr;

    void NotifyComponentAdded(Component* component);

};