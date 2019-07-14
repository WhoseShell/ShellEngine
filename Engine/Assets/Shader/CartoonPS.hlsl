#include"../../RenderPipeline/ShaderLibrary/Core.hlsli"

Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normalWS:NORMAL;
	float2 uv:TEXCOORD0;
	float3 normalVS:TEXCOORD1;
	float3 posWS:TEXCOORD2;
};

float4 main(PSInput input):SV_Target
{
	float4 color = DiffuseTexture.Sample(DiffuseSampler, input.uv);
	if (dot(input.normalVS, float3(0.0f, 0.0f, 1.0f)) <= 0.5f)
	{
		color = float4(0.0f, 0.0f, 0.0f, 1.0f);
	}

	return color;
}