// GLOBALS //
cbuffer MatrixBuffer
{
    matrix worldMatrix;
    matrix viewMatrix;
    matrix projectionMatrix;
};

cbuffer LightBuffer
{
    float4 diffuseColor;
    float3 lightDirection;
    float specularPower;

    float4 ambientColor;
    float4 specularColor;

    float3 cameraPosition;
    float padding;
};

// [0]: 색상 텍스처, [1]: 노멀맵
Texture2D shaderTextures[2];
SamplerState SampleType;


// TYPEDEFS //
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
};


// Vertex Shader
PixelInputType BumpMapVertexShader(VertexInputType input)
{
    PixelInputType output;

    input.position.w = 1.0f;

    // 월드 위치와 화면에 표시할 위치 계산
    float4 worldPosition = mul(input.position, worldMatrix);

    output.worldPosition = worldPosition.xyz;
    output.position = mul(worldPosition, viewMatrix);
    output.position = mul(output.position, projectionMatrix);

    output.tex = input.tex;

    // 방향 벡터를 월드 공간으로 변환
    // 양수 균일 스케일을 사용하는 모델 기준
    output.normal = normalize(
        mul(input.normal, (float3x3) worldMatrix)
    );

    output.tangent = normalize(
        mul(input.tangent, (float3x3) worldMatrix)
    );

    output.binormal = normalize(
        mul(input.binormal, (float3x3) worldMatrix)
    );

    return output;
}


// Pixel Shader
float4 BumpMapPixelShader(PixelInputType input) : SV_TARGET
{
    float4 textureColor;
    float4 bumpMap;
    float3 bumpNormal;
    float3 lightDir;
    float lightIntensity;
    float4 color;

    float3 viewDirection;
    float3 reflection;
    float4 specular;

    textureColor =
        shaderTextures[0].Sample(SampleType, input.tex);

    bumpMap =
        shaderTextures[1].Sample(SampleType, input.tex);

    // 노멀맵 값을 [0, 1]에서 [-1, 1]로 변환
    bumpMap = (bumpMap * 2.0f) - 1.0f;

    input.normal = normalize(input.normal);
    input.tangent = normalize(input.tangent);
    input.binormal = normalize(input.binormal);

    // 노멀맵의 방향을 월드 공간의 법선으로 변환
    bumpNormal =
        (bumpMap.x * input.tangent) +
        (bumpMap.y * input.binormal) +
        (bumpMap.z * input.normal);

    bumpNormal = normalize(bumpNormal);

    lightDir = normalize(-lightDirection);

    // 노멀맵 법선으로 확산광 계산
    lightIntensity =
        saturate(dot(bumpNormal, lightDir));

    color = ambientColor + diffuseColor * lightIntensity;
    color = color * textureColor;

    specular = float4(0.0f, 0.0f, 0.0f, 0.0f);

    // 빛을 받는 면에 Phong 정반사 적용
    if (lightIntensity > 0.0f)
    {
        viewDirection = normalize(
            cameraPosition - input.worldPosition
        );

        reflection = normalize(
            2.0f * lightIntensity * bumpNormal - lightDir
        );

        specular = specularColor * pow(
            saturate(dot(reflection, viewDirection)),
            specularPower
        );
    }

    color.rgb = saturate(color.rgb + specular.rgb);
    color.a = textureColor.a;

    return color;
}