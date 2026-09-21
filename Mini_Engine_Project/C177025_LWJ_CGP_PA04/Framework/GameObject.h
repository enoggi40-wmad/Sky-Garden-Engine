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

    // GameObject에 Component를 추가
    void AddComponent(Component* newComponent);

    // 이름 반환
    std::string GetName() const;

public:
    // 모든 GameObject가 기본으로 가지는 Transform
    Transform transform;

private:
    // GameObject 이름
    std::string name;

    // 이 GameObject에 붙어있는 Component 목록
    std::vector<Component*> components;
};