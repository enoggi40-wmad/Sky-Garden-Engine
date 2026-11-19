#pragma once

#include "Light.h"
#include "LightData.h"

#include <vector>
#include <type_traits>

class LightManager
{
public:
    LightManager() = default;
    ~LightManager() = default;

	// m_Lights에 Light를 등록/해제하는 함수
    void RegisterLight(Light* light);
    void UnregisterLight(Light* light);
    
    //LightManager가 가지고 있는 광원 목록을 복사하지 않고 반환하되, 
    //외부에서 목록 자체는 수정할 수 없도록 하는 조회 함수
    const std::vector<Light*>& GetLights() const;

	// 이미 등록된 광원 중에서 특정 타입의 광원만을 반환하는 템플릿 함수
    template<typename T>
    std::vector<T*> GetLightsOfType() const
    {
        static_assert(std::is_base_of_v<Light, T>,
            "T must inherit from Light");

        std::vector<T*> result;

        for (Light* light : m_Lights)
        {
            if (T* casted = dynamic_cast<T*>(light))
            {
                result.push_back(casted);
            }
        }

        return result;
    }

    LightData GetLightData() const;

	

private:
    std::vector<Light*> m_Lights;
};
