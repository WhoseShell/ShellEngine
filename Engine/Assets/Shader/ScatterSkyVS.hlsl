#include"../../RenderPipeline/ShaderLibrary/Core.hlsli"

// 用作顶点着色器输入的每个顶点的数据。
struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 uv : UV;
};

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float2 uv: UV;
	float3 posWS : TEXCOORD0;
};

PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output = (PixelShaderInput)0;
	float4 pos = float4(input.pos, 1.0f);
	float4 posWS = mul(model, pos);
	float4 posVS = mul(view, posWS);
	float4 posClip = mul(projection, posVS);
	output.posWS = input.pos.xyz;
	output.pos = posClip;
	output.uv = input.uv;

	return output;
};