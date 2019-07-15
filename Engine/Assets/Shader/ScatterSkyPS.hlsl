//#define IS_RENDERING_SKY 1

#include"../../RenderPipeline/ShaderLibrary/Core.hlsli"

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 uv: UV;
	float3 posWS : TEXCOORD0;
};

float4 main(PixelShaderInput input):SV_Target
{
	float4 color = 0.5f;
	color.a = 1.0f;

	color.xyz = MixFog(color.xyz, input.posWS);

	return color;
};