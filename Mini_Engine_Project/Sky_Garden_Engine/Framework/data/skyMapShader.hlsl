cbuffer cbPerObject
{
    float4x4 WVP;
    float4x4 World;
};

SamplerState ObjSamplerState;
TextureCube SkyMap;

struct SKYMAP_VS_OUTPUT
{
    float4 Pos : SV_POSITION;
    float3 texCoord : TEXCOORD;
};

SKYMAP_VS_OUTPUT SKYMAP_VS(
	float3 inPos : POSITION,
	float2 inTexCoord : TEXCOORD,
	float3 normal : NORMAL
)
{
    SKYMAP_VS_OUTPUT output = (SKYMAP_VS_OUTPUT) 0;

	// Sky를 항상 가장 먼 깊이에 두기 위해 z = w
    output.Pos =
		mul(float4(inPos, 1.0f), WVP).xyww;

	// CubeMap은 UV가 아니라 3D 방향벡터로 샘플링
    output.texCoord = inPos;

    return output;
}

float4 SKYMAP_PS(SKYMAP_VS_OUTPUT input) : SV_TARGET
{
    return SkyMap.Sample(
		ObjSamplerState,
		input.texCoord
	);
}