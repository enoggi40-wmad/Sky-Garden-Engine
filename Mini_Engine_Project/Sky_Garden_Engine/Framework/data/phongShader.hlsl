
#define MAX_POINT_LIGHTS 16
#define MAX_SPOT_LIGHTS 16

// Vertex Shader Constant Buffers
cbuffer MatrixBuffer : register(b0)
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer CameraBuffer : register(b1)
{
    float3 cameraPosition;
    float cameraPadding;
};

// Pixel Shader Constant Buffers
cbuffer LightBuffer : register(b0)
{
    float4 ambientColor;
    float4 specularColor;

    float specularPower;
    float3 lightPadding;
};

// DirectionalLight
struct DirectionalLightData
{
    // xyz: Direction, w: Intensity
    float4 directionIntensity;

    float4 diffuseColor;
};

// PointLight
struct PointLightData
{
    // xyz: Position, w: Range
    float4 positionRange;

    // rgb: DiffuseColor, a: Intensity
    float4 diffuseColorIntensity;

    // xyz: Attenuation
    float4 attenuation;
};

// SpotLight
struct SpotLightData
{
    // xyz: Position, w: Range
    float4 positionRange;

    // xyz: Direction, w: InnerCos
    float4 directionInnerCos;

    // rgb: DiffuseColor, a: Intensity
    float4 diffuseColorIntensity;

    // xyz: Attenuation, w: OuterCos
    float4 attenuationOuterCos;
};

// Scene의 전체 광원 데이터
cbuffer LightDataBuffer : register(b1)
{
    DirectionalLightData directionalLight;

    PointLightData pointLights[MAX_POINT_LIGHTS];
    SpotLightData spotLights[MAX_SPOT_LIGHTS];

    int pointLightCount;
    int spotLightCount;
    float2 countPadding;
};

Texture2D shaderTexture : register(t0);
SamplerState SampleType : register(s0);

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;

    float3 normal : NORMAL;
    float3 viewDirection : TEXCOORD1;
    float3 worldPosition : TEXCOORD2;
};

// Vertex Shader
PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    // World Position
    float4 worldPos =
        mul(input.position, worldMatrix);

    // Clip Position
    output.position = mul(worldPos, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.worldPosition = worldPos.xyz;

    output.tex = input.tex;

    // World Normal
    output.normal =
        mul(input.normal, (float3x3) worldMatrix);

    // Camera Direction
    output.viewDirection =
        cameraPosition - worldPos.xyz;

    return output;
}

// Specular 계산
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

    float3 reflection =
        reflect(-lightDir, normal);

    float factor = pow(
        saturate(dot(reflection, viewDir)),
        max(specularPower, 0.0001f)
    );

    return specularColor.rgb * lightColor * factor;
}

// 거리 감쇠 계산
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

    // Range 끝부분에서 부드럽게 감소
    float rangeFactor =
        saturate(1.0f - distance / range);

    return distanceAttenuation *
        rangeFactor * rangeFactor;
}

// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor =
        shaderTexture.Sample(SampleType, input.tex);

    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(input.viewDirection);

    // Phong 세 요소
    float3 ambient = ambientColor.rgb;
    float3 diffuse = float3(0.0f, 0.0f, 0.0f);
    float3 specular = float3(0.0f, 0.0f, 0.0f);

    // 1. DirectionalLight
    if (directionalLight.directionIntensity.w > 0.0f)
    {
        float3 lightDir = normalize(
            -directionalLight.directionIntensity.xyz
        );

        float3 lightColor =
            directionalLight.diffuseColor.rgb *
            directionalLight.directionIntensity.w;

        float NdotL = saturate(dot(normal, lightDir));

        diffuse += lightColor * NdotL;

        specular += CalculateSpecular(
            normal, lightDir, viewDir, lightColor
        );
    }

    // 2. PointLight
    [loop]
    for (int i = 0; i < pointLightCount; i++)
    {
        PointLightData light = pointLights[i];

        float3 toLight =
            light.positionRange.xyz - input.worldPosition;

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

        diffuse += lightColor * NdotL;

        specular += CalculateSpecular(
            normal, lightDir, viewDir, lightColor
        );
    }

    // 3. SpotLight
    [loop]
    for (int i = 0; i < spotLightCount; i++)
    {
        SpotLightData light = spotLights[i];

        float3 toLight =
            light.positionRange.xyz - input.worldPosition;

        float distance = length(toLight);

        float3 lightDir =
            toLight / max(distance, 0.0001f);

        // 광원에서 픽셀을 향하는 방향
        float3 lightToPixel = -lightDir;

        float3 spotDirection =
            normalize(light.directionInnerCos.xyz);

        float cosAngle =
            dot(lightToPixel, spotDirection);

        float innerCos =
            light.directionInnerCos.w;

        float outerCos =
            light.attenuationOuterCos.w;

        // 조사각의 안쪽과 바깥쪽 사이에서 감쇠
        float coneFactor = saturate(
            (cosAngle - outerCos) /
            max(innerCos - outerCos, 0.0001f)
        );

        coneFactor =
            coneFactor * coneFactor *
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

        diffuse += lightColor * NdotL;

        specular += CalculateSpecular(
            normal, lightDir, viewDir, lightColor
        );
    }

    // 최종 Phong Lighting
    float3 lighting = ambient + diffuse;

    float3 finalColor =
        textureColor.rgb * lighting + specular;

    return float4(saturate(finalColor), textureColor.a);
}
