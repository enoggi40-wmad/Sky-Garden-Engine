#include "GameObject.h"
#include <algorithm>
#include <cmath>

// 생성자
GameObject::GameObject(const std::string& objectName)
    : name(objectName)
{
}

// 소멸자
GameObject::~GameObject()
{
	// 부모와의 연결 해제
    SetParent(nullptr, false);

    while (!m_Children.empty())
    {
        m_Children.back()->SetParent(nullptr, false);
    }

    // 자신이 가진 Component 삭제
    for (Component* component : components)
    {
        delete component;
    }

    components.clear();
}


bool GameObject::SetParent(
    GameObject* newParent,
    bool keepWorldTransform)
{
    // 1. 자기 자신은 부모가 될 수 없다.
    if (newParent == this)
    {
        return false;
    }

    // 2. 이미 같은 부모라면 그대로 유지
    if (m_Parent == newParent)
    {
        return true;
    }

    // 3. 자신의 자손을 부모로 삼는 것을 방지
    for (GameObject* ancestor = newParent;
        ancestor != nullptr;
        ancestor = ancestor->m_Parent)
    {
        if (ancestor == this)
        {
            return false;
        }
    }

    // 4. 부모 변경 후 사용할 로컬 행렬
    XMMATRIX newLocalMatrix = transform.GetLocalMatrix();

    if (keepWorldTransform)
    {
        // 부모 변경 전 현재 월드 행렬 저장
        XMMATRIX oldWorldMatrix =
            transform.GetWorldMatrix();

        // 새 부모의 월드 행렬
        XMMATRIX parentWorldMatrix =
            XMMatrixIdentity();

        if (newParent)
        {
            parentWorldMatrix =
                newParent->transform.GetWorldMatrix();
        }

        // 부모 월드 행렬의 행렬식 계산
        float determinant =
            XMVectorGetX(
                XMMatrixDeterminant(parentWorldMatrix)
            );

        // 역행렬을 만들 수 없다면 부모 변경 거부
        if (!std::isfinite(determinant) ||
            determinant == 0.0f)
        {
            return false;
        }

        // 기존 월드 변환을 유지하기 위한
        // 새 부모 기준 로컬 행렬 계산
        newLocalMatrix =
            oldWorldMatrix *
            XMMatrixInverse(
                nullptr,
                parentWorldMatrix
            );

        // 새 로컬 행렬을 위치, 회전, 크기로 분해
        // 실패하면 부모 관계를 변경하지 않는다.
        if (!transform.TrySetLocalMatrix(newLocalMatrix))
        {
            return false;
        }
    }

    // 5. 기존 부모의 자식 목록에서 제거
    if (m_Parent)
    {
        auto& siblings = m_Parent->m_Children;

        siblings.erase(
            std::remove(
                siblings.begin(),
                siblings.end(),
                this
            ),
            siblings.end()
        );
    }

    // 6. 새로운 부모의 자식 목록에 추가
    if (newParent)
    {
        newParent->m_Children.push_back(this);
    }

    // 7. GameObject의 부모 포인터 변경
    m_Parent = newParent;

    // 8. Transform의 부모 포인터도 변경
    if (newParent)
    {
        transform.m_Parent =
            &newParent->transform;
    }
    else
    {
        transform.m_Parent = nullptr;
    }

    return true;
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
