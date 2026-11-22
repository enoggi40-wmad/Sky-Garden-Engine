
#include "LightingCommon.hlsli"

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

    float4 worldPos = mul(input.position, worldMatrix);

    output.position = mul(worldPos, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.worldPosition = worldPos.xyz;
    output.tex = input.tex;

    // 기존 World Normal 계산 유지
    output.normal =
        mul(input.normal, (float3x3) worldMatrix);

    output.viewDirection =
        cameraPosition - worldPos.xyz;

    return output;
}

// Pixel Shader
float4 LightPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor =
        shaderTexture.Sample(SampleType, input.tex);

    float3 normal = normalize(input.normal);
    float3 viewDir = normalize(input.viewDirection);

    LightingResult lighting = CalculateLighting(
        input.worldPosition,
        normal,
        viewDir
    );

    return ComposeLitColor(textureColor, lighting);
}
