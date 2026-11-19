#include "LightManager.h"
#include "DirectionalLight.h"
#include "PointLight.h"
#include "SpotLight.h"

#include <DirectXMath.h>
#include <cmath>

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


LightData LightManager::GetLightData() const
{
    LightData data = {};

    // DirectionalLight
    // 현재는 첫 번째 DirectionalLight 하나만 사용
    for (DirectionalLight* light :
        GetLightsOfType<DirectionalLight>())
    {
        DirectX::XMFLOAT3 direction =
            light->GetDirection();

        data.directionalLight.directionIntensity =
        {
            direction.x,
            direction.y,
            direction.z,
            light->GetIntensity()
        };

        data.directionalLight.diffuseColor =
            light->GetDiffuseColor();

        break;
    }

    // PointLight
    for (PointLight* light :
        GetLightsOfType<PointLight>())
    {
        if (data.pointLightCount >= MAX_POINT_LIGHTS)
            break;

        int index = data.pointLightCount++;

        DirectX::XMFLOAT3 position =
            light->GetPosition();

        DirectX::XMFLOAT4 color =
            light->GetDiffuseColor();

        DirectX::XMFLOAT3 attenuation =
            light->GetAttenuation();

        data.pointLights[index].positionRange =
        {
            position.x,
            position.y,
            position.z,
            light->GetRange()
        };

        data.pointLights[index].diffuseColorIntensity =
        {
            color.x,
            color.y,
            color.z,
            light->GetIntensity()
        };

        data.pointLights[index].attenuation =
        {
            attenuation.x,
            attenuation.y,
            attenuation.z,
            0.0f
        };
    }

    // SpotLight
    for (SpotLight* light :
        GetLightsOfType<SpotLight>())
    {
        if (data.spotLightCount >= MAX_SPOT_LIGHTS)
            break;

        int index = data.spotLightCount++;

        DirectX::XMFLOAT3 position =
            light->GetPosition();

        DirectX::XMFLOAT3 direction =
            light->GetDirection();

        DirectX::XMFLOAT4 color =
            light->GetDiffuseColor();

        DirectX::XMFLOAT3 attenuation =
            light->GetAttenuation();

        // 전체 조사각(도)을 반각(라디안)으로 변환
        float innerAngle =
            DirectX::XMConvertToRadians(
                light->GetInnerAngle() * 0.5f
            );

        float outerAngle =
            DirectX::XMConvertToRadians(
                light->GetOuterAngle() * 0.5f
            );

        float innerCos = std::cos(innerAngle);
        float outerCos = std::cos(outerAngle);

        data.spotLights[index].positionRange =
        {
            position.x,
            position.y,
            position.z,
            light->GetRange()
        };

        data.spotLights[index].directionInnerCos =
        {
            direction.x,
            direction.y,
            direction.z,
            innerCos
        };

        data.spotLights[index].diffuseColorIntensity =
        {
            color.x,
            color.y,
            color.z,
            light->GetIntensity()
        };

        data.spotLights[index].attenuationOuterCos =
        {
            attenuation.x,
            attenuation.y,
            attenuation.z,
            outerCos
        };
    }

    return data;
}

