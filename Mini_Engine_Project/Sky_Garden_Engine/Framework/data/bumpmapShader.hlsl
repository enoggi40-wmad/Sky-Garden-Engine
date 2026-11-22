
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
// Textures
// ==========================================

// t0: Diffuse Texture
// t1: Normal Map
Texture2D shaderTextures[2] : register(t0);

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

PixelInputType BumpMapVertexShader(
    VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    float4 worldPos =
        mul(input.position, worldMatrix);

    output.position =
        mul(worldPos, viewMatrix);

    output.position =
        mul(output.position, projectionMatrix);

    output.worldPosition = worldPos.xyz;
    output.tex = input.tex;

    // 기존 World Space TBN 변환 방식 유지
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
// Normal Mapping
// ==========================================

float3 CalculateBumpNormal(PixelInputType input)
{
    // Normal Map 샘플링
    float3 normalMap =
        shaderTextures[1].Sample(
            SampleType, input.tex
        ).rgb;

    // [0, 1] → [-1, 1]
    normalMap = normalMap * 2.0f - 1.0f;

    // World Space TBN
    float3 N = normalize(input.normal);
    float3 T = normalize(input.tangent);
    float3 B = normalize(input.binormal);

    // Tangent Space → World Space
    float3 bumpNormal =
        normalMap.x * T +
        normalMap.y * B +
        normalMap.z * N;

    return normalize(bumpNormal);
}

// ==========================================
// Pixel Shader
// ==========================================

float4 BumpMapPixelShader(
    PixelInputType input) : SV_TARGET
{
    // 1. Diffuse Texture
    float4 textureColor =
        shaderTextures[0].Sample(
            SampleType, input.tex
        );

    // 2. Normal Map → World Space Normal
    float3 normal = CalculateBumpNormal(input);

    // 3. 카메라 방향
    float3 viewDir =
        normalize(input.viewDirection);

    // 4. 공통 Lighting 계산
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
