
#include "LightingCommon.hlsli"

// ==========================================
// Vertex Shader Constant Buffers
// ==========================================

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

// ==========================================
// Pixel Shader Constant Buffers
// ==========================================

// LightingCommon.hlsli에서 b0, b1 사용
cbuffer TextureOptionBuffer : register(b2)
{
    int useAlphaMap;
    int useNormalMap;
    float2 textureOptionPadding;
};

// ==========================================
// Textures
// ==========================================

Texture2D diffuseTexture : register(t0);
Texture2D secondTexture : register(t1);
Texture2D alphaMap : register(t2);
Texture2D normalMap : register(t3);

SamplerState SampleType : register(s0);

// ==========================================
// Vertex Input / Pixel Input
// ==========================================

struct VertexInputType
{
    float4 position : POSITION;
    float2 tex : TEXCOORD0;
    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;
};

struct PixelInputType
{
    float4 position : SV_POSITION;
    float2 tex : TEXCOORD0;

    float3 normal : NORMAL;
    float3 tangent : TANGENT;
    float3 binormal : BINORMAL;

    float3 worldPosition : TEXCOORD1;
    float3 viewDirection : TEXCOORD2;
};

// ==========================================
// Vertex Shader
// ==========================================

PixelInputType LightVertexShader(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    float4 worldPos =
        mul(input.position, worldMatrix);

    output.position = mul(worldPos, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.worldPosition = worldPos.xyz;
    output.tex = input.tex;

    // 기존 World Space TBN 변환 방식
    output.normal = normalize(
        mul(input.normal, (float3x3) worldMatrix)
    );

    output.tangent = normalize(
        mul(input.tangent, (float3x3) worldMatrix)
    );

    output.binormal = normalize(
        mul(input.binormal, (float3x3) worldMatrix)
    );

    output.viewDirection =
        cameraPosition - worldPos.xyz;

    return output;
}

// ==========================================
// Texture Blending
// ==========================================

float4 CalculateTextureColor(PixelInputType input)
{
    float4 color1 =
        diffuseTexture.Sample(SampleType, input.tex);

    if (useAlphaMap != 0)
    {
        float4 color2 =
            secondTexture.Sample(SampleType, input.tex);

        float4 alphaValue =
            alphaMap.Sample(SampleType, input.tex);

        // 기존 AlphaMapShader 혼합 규칙 유지
        return saturate(
            alphaValue * color1 +
            (1.0f - alphaValue) * color2
        );
    }

    return color1;
}

// ==========================================
// Normal Mapping
// ==========================================

float3 CalculateSurfaceNormal(PixelInputType input)
{
    float3 N = normalize(input.normal);

    if (useNormalMap == 0)
    {
        return N;
    }

    float3 mappedNormal =
        normalMap.Sample(SampleType, input.tex).rgb;

    mappedNormal =
        mappedNormal * 2.0f - 1.0f;

    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);

    float3 surfaceNormal =
        mappedNormal.x * T +
        mappedNormal.y * B +
        mappedNormal.z * N;

    return normalize(surfaceNormal);
}

// ==========================================
// Pixel Shader
// ==========================================

float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    // 1. Diffuse / Alpha Mapping
    float4 textureColor =
        CalculateTextureColor(input);

    // 2. 기본 Normal / Normal Mapping
    float3 normal =
        CalculateSurfaceNormal(input);

    // 3. View Direction
    float3 viewDir =
        normalize(input.viewDirection);

    // 4. 공통 Phong Lighting
    LightingResult lighting = CalculateLighting(
        input.worldPosition,
        normal,
        viewDir
    );

    // 5. 최종 색상
    return ComposeLitColor(
        textureColor,
        lighting
    );
}
