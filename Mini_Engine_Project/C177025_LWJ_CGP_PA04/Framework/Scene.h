#pragma once

#include <vector>

class GameObject;

class Scene
{
public:
    Scene();
    ~Scene();

    // Scene 안에 GameObject를 추가
    void AddGameObject(GameObject* gameObject);

    // Scene 안의 모든 GameObject를 매 프레임 업데이트
    void Update(float deltaTime);

private:
    // 현재 Scene에 들어있는 GameObject 목록
    std::vector<GameObject*> gameObjects;
};