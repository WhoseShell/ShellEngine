#include"../../RenderPipeline/ShaderLibrary/Core.hlsli"

cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;

	float time;
};

struct VSInput
{
	float3 position:POSITION;
	float4 normal:NORMAL;
	float4 tangent:TANGENT;
	float2 uv:TEXCOORD0;
};

struct PSInput
{
	float4 position : SV_POSITION;
	float3 normalWS:NORMAL;
	float2 uv:TEXCOORD0;
	float3 normalVS:TEXCOORD1;
};

PSInput main(VSInput input)
{
	PSInput output = (PSInput)0;

	/**********************************************************************/
	//坐标点空间转换

	float4 posModel = float4(input.position, 1.0f);
	float4 posWS = mul(model, posModel);
	float4 posVS = mul(view, posWS);
	float4 posClip = mul(projection, posVS);
	output.position = posClip;

	/**********************************************************************/
	//法线空间转换
	//float4 biTangent = cross(input.tangent, input.normal);//DX使用左手坐标系

	output.normalWS = mul(model, input.normal).xyz;
	output.normalVS = mul(view, output.normalWS).xyz;

	/**********************************************************************/
	output.uv = input.uv;

	return output;
}