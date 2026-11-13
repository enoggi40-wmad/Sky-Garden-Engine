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


GameObject* GameObject::GetParent() const
{
    return m_Parent;
}


bool GameObject::SetParent(
    GameObject* newParent,
    bool keepWorldTransform)
{
    // 1. 자기 자신을 부모로 설정하는 것을 방지
    if (newParent == this)
    {
        return false;
    }

    // 2. 이미 같은 부모라면 변경하지 않음
    if (m_Parent == newParent)
    {
        return true;
    }

    // 3. 순환 참조 방지
    // 새 부모의 조상을 확인하여 자기 자신이 있는지 검사
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
    XMMATRIX newLocalMatrix =
        transform.GetLocalMatrix();

    // 부모를 변경해도 기존 World Transform을 유지하는 경우
    if (keepWorldTransform)
    {
        // 부모 변경 전 자식의 World Matrix 저장
        XMMATRIX oldWorldMatrix =
            transform.GetWorldMatrix();

        // 새 부모의 World Matrix 가져오기
        // 부모가 없으면 단위 행렬 사용
        XMMATRIX parentWorldMatrix =
            XMMatrixIdentity();

        if (newParent)
        {
            parentWorldMatrix =
                newParent->transform.GetWorldMatrix();
        }

        // 부모 행렬의 역행렬을 구할 수 있는지 검사
        float determinant =
            XMVectorGetX(
                XMMatrixDeterminant(parentWorldMatrix)
            );

        // 역행렬 계산이 불가능하면 부모 변경 취소
        if (!std::isfinite(determinant) ||
            determinant == 0.0f)
        {
            return false;
        }

        // 5. 새로운 Local Matrix 계산
        // 기존 World Transform을 유지하기 위해
        // 새 부모의 변환을 역행렬로 제거
        newLocalMatrix =
            oldWorldMatrix *
            XMMatrixInverse(
                nullptr,
                parentWorldMatrix
            );

        // 6. Local Matrix를 Position, Rotation, Scale로 분해
        // Transform은 행렬이 아닌 개별 값으로 저장하기 때문
        if (!transform.TrySetLocalMatrix(newLocalMatrix))
        {
            return false;
        }
    }

    // 7. 기존 부모의 자식 목록에서 자신을 제거
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

    // 8. 새 부모의 자식 목록에 자신을 추가
    if (newParent)
    {
        newParent->m_Children.push_back(this);
    }

    // 9. GameObject의 부모 포인터 변경
    m_Parent = newParent;

    // 10. Transform의 부모 포인터도 변경
    // GetWorldMatrix()에서 부모 변환을 적용하기 위해 필요
    if (newParent)
    {
        transform.m_Parent =
            &newParent->transform;
    }
    else
    {
        transform.m_Parent = nullptr;
    }

    // 11. 부모 변경으로 World Matrix 캐시 무효화
    transform.m_WorldDirty = true;

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
