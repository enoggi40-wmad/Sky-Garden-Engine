#pragma once

#include <DirectXMath.h>

constexpr int MAX_POINT_LIGHTS = 16;
constexpr int MAX_SPOT_LIGHTS = 16;

// DirectionalLight 데이터
struct DirectionalLightData
{
    // xyz: Direction, w: Intensity
    DirectX::XMFLOAT4 directionIntensity = {};

    // rgba: Color
    DirectX::XMFLOAT4 diffuseColor = {};
};

// PointLight 데이터
struct PointLightData
{
    // xyz: Position, w: Range
    DirectX::XMFLOAT4 positionRange = {};

    // xyz: Color, w: Intensity
    DirectX::XMFLOAT4 diffuseColorIntensity = {};

    // xyz: Attenuation, w: Padding
    DirectX::XMFLOAT4 attenuation = {};
};

// SpotLight 데이터
struct SpotLightData
{
    // xyz: Position, w: Range
    DirectX::XMFLOAT4 positionRange = {};

    // xyz: Direction, w: InnerCos
    DirectX::XMFLOAT4 directionInnerCos = {};

    // xyz: Color, w: Intensity
    DirectX::XMFLOAT4 diffuseColorIntensity = {};

    // xyz: Attenuation, w: OuterCos
    DirectX::XMFLOAT4 attenuationOuterCos = {};
};

// GPU Constant Buffer용 데이터
struct alignas(16) LightData
{
    DirectionalLightData directionalLight = {};

    PointLightData pointLights[MAX_POINT_LIGHTS] = {};
    SpotLightData spotLights[MAX_SPOT_LIGHTS] = {};

    int pointLightCount = 0;
    int spotLightCount = 0;

    float padding[2] = {};
};

static_assert(sizeof(LightData) % 16 == 0,
    "LightData size must be a multiple of 16 bytes");
