#include "GameObject.h"

// 생성자
GameObject::GameObject(const std::string& objectName)
    : name(objectName)
{
}

// 소멸자
GameObject::~GameObject()
{
    // GameObject가 사라질 때
    // 가지고 있던 Component들도 삭제
    for (Component* component : components)
    {
        delete component;
    }

    components.clear();
}

// 초기화
void GameObject::Initialize()
{
    // 붙어있는 모든 Component 초기화
    for (Component* component : components)
    {
        component->Initialize();
    }
}

// 매 프레임 업데이트
void GameObject::Update(float deltaTime)
{
    // 붙어있는 모든 Component의 Update 호출
    for (Component* component : components)
    {
        component->Update(deltaTime);
    }
}

// 이름 반환
std::string GameObject::GetName() const
{
    return name;
}

// 기존 AddComponent 방식
//void AddComponent(Component* newComponent)
//{
//    components.push_back(newComponent);
//}