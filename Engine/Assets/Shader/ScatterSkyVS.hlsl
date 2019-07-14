#include"../../RenderPipeline/ShaderLibrary/Core.hlsli"

// ����������ɫ�������ÿ����������ݡ�
struct VertexShaderInput
{
	float3 pos : POSITION;
	float2 uv : UV;
};

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
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