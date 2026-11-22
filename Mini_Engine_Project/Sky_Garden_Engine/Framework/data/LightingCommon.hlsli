
#ifndef LIGHTING_COMMON_HLSLI
#define LIGHTING_COMMON_HLSLI

#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

// =============================================
// Light Data
// =============================================

struct DirectionalLightData
{
    float4 directionIntensity;
    float4 diffuseColor;
};

struct PointLightData
{
    float4 positionRange;
    float4 diffuseColorIntensity;
    float4 attenuation;
};

struct SpotLightData
{
    float4 positionRange;
    float4 directionInnerCos;
    float4 diffuseColorIntensity;
    float4 attenuationOuterCos;
};

// PS b0: Material 조명 속성
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 specularColor;

    float specularPower;
    float3 lightPadding;

    float4 emissiveColor;
    float emissiveIntensity;
    float3 emissivePadding;
};

// PS b1: Scene 광원 데이터
cbuffer LightDataBuffer : register(b1)
{
    DirectionalLightData directionalLight;

    PointLightData pointLights[MAX_POINT_LIGHTS];
    SpotLightData spotLights[MAX_SPOT_LIGHTS];

    int pointLightCount;
    int spotLightCount;
    float2 countPadding;
};

// =============================================
// Lighting Result
// =============================================

struct LightingResult
{
    float3 ambient;
    float3 diffuse;
    float3 specular;
};

// =============================================
// Specular
// =============================================

float3 CalculateSpecular(
    float3 normal,
    float3 lightDir,
    float3 viewDir,
    float3 lightColor)
{
    float diffuseFactor =
        saturate(dot(normal, lightDir));

    if (diffuseFactor <= 0.0f)
    {
        return float3(0.0f, 0.0f, 0.0f);
    }

    float3 reflection = reflect(-lightDir, normal);

    float factor = pow(
        saturate(dot(reflection, viewDir)),
        max(specularPower, 0.0001f)
    );

    return specularColor.rgb * lightColor * factor;
}

// =============================================
// Distance Attenuation
// =============================================

float CalculateAttenuation(
    float distance,
    float range,
    float3 attenuation)
{
    if (range <= 0.0f || distance >= range)
    {
        return 0.0f;
    }

    float denominator =
        attenuation.x +
        attenuation.y * distance +
        attenuation.z * distance * distance;

    float distanceAttenuation =
        1.0f / max(denominator, 0.0001f);

    float rangeFactor =
        saturate(1.0f - distance / range);

    return distanceAttenuation *
           rangeFactor * rangeFactor;
}

// =============================================
// All Lights
// =============================================

LightingResult CalculateLighting(
    float3 worldPosition,
    float3 normal,
    float3 viewDir)
{
    LightingResult result;

    result.ambient = ambientColor.rgb;
    result.diffuse = float3(0.0f, 0.0f, 0.0f);
    result.specular = float3(0.0f, 0.0f, 0.0f);

    // 1. Directional Light
    if (directionalLight.directionIntensity.w > 0.0f)
    {
        float3 lightDir = normalize(
            -directionalLight.directionIntensity.xyz
        );

        float3 lightColor =
            directionalLight.diffuseColor.rgb *
            directionalLight.directionIntensity.w;

        float NdotL = saturate(dot(normal, lightDir));

        result.diffuse += lightColor * NdotL;

        result.specular += CalculateSpecular(
            normal, lightDir, viewDir, lightColor
        );
    }

    // 2. Point Lights
    [loop]
    for (int pointIndex = 0; pointIndex < pointLightCount; pointIndex++)
    {
        PointLightData light = pointLights[pointIndex];

        float3 toLight =
            light.positionRange.xyz - worldPosition;

        float distance = length(toLight);
        float3 lightDir =
            toLight / max(distance, 0.0001f);

        float attenuation = CalculateAttenuation(
            distance,
            light.positionRange.w,
            light.attenuation.xyz
        );

        float3 lightColor =
            light.diffuseColorIntensity.rgb *
            light.diffuseColorIntensity.w *
            attenuation;

        float NdotL = saturate(dot(normal, lightDir));

        result.diffuse += lightColor * NdotL;

        result.specular += CalculateSpecular(
            normal, lightDir, viewDir, lightColor
        );
    }

    // 3. Spot Lights
    [loop]
    for (int spotIndex = 0; spotIndex < spotLightCount; spotIndex++)
    {
        SpotLightData light = spotLights[spotIndex];

        float3 toLight =
            light.positionRange.xyz - worldPosition;

        float distance = length(toLight);
        float3 lightDir =
            toLight / max(distance, 0.0001f);

        float3 lightToPixel = -lightDir;
        float3 spotDirection =
            normalize(light.directionInnerCos.xyz);

        float cosAngle =
            dot(lightToPixel, spotDirection);

        float innerCos = light.directionInnerCos.w;
        float outerCos = light.attenuationOuterCos.w;

        float coneFactor = saturate(
            (cosAngle - outerCos) /
            max(innerCos - outerCos, 0.0001f)
        );

        coneFactor = coneFactor * coneFactor *
                     (3.0f - 2.0f * coneFactor);

        float attenuation = CalculateAttenuation(
            distance,
            light.positionRange.w,
            light.attenuationOuterCos.xyz
        );

        float3 lightColor =
            light.diffuseColorIntensity.rgb *
            light.diffuseColorIntensity.w *
            attenuation *
            coneFactor;

        float NdotL = saturate(dot(normal, lightDir));

        result.diffuse += lightColor * NdotL;

        result.specular += CalculateSpecular(
            normal, lightDir, viewDir, lightColor
        );
    }

    return result;
}

// =============================================
// Final Color
// =============================================

float4 ComposeLitColor(
    float4 textureColor,
    LightingResult lighting)
{
    float3 emissive =
        emissiveColor.rgb * emissiveIntensity;

    float3 finalColor =
        textureColor.rgb *
        (lighting.ambient + lighting.diffuse)
        + lighting.specular
        + emissive;

    return float4(
        saturate(finalColor),
        textureColor.a
    );
}

#endif
