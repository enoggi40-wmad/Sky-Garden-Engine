
#include "LightManager.h"

#include <algorithm>

void LightManager::RegisterLight(Light* light)
{
    if (light == nullptr)
        return;

    // 중복 등록 방지
    auto it = std::find(
        m_Lights.begin(),
        m_Lights.end(),
        light
    );

    if (it != m_Lights.end())
        return;

    m_Lights.push_back(light);
}

void LightManager::UnregisterLight(Light* light)
{
    auto it = std::remove(
        m_Lights.begin(),
        m_Lights.end(),
        light
    );

    m_Lights.erase(it, m_Lights.end());
}

const std::vector<Light*>& LightManager::GetLights() const
{
    return m_Lights;
}
