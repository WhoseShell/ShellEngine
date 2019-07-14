#include"../../RenderPipeline/ShaderLibrary/Core.hlsli"

struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 uv: UV;
	float3 posWS : TEXCOORD0;
};

float4 main(PixelShaderInput input):SV_Target
{
	return 0.5f;
};