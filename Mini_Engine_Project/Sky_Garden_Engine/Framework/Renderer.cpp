#include "Renderer.h"
#include "GameObject.h"

Renderer::Renderer(GameObject* owner)
	: Component(owner)
{
	m_IsTransparent = false;
}

Renderer::~Renderer()
{
}

void Renderer::SetTransparent(bool transparent)
{
	m_IsTransparent = transparent;
}

bool Renderer::IsTransparent() const
{
	return m_IsTransparent;
}

DirectX::XMFLOAT3 Renderer::GetPosition() const
{
	return gameObject->transform.GetLocalPosition();
}