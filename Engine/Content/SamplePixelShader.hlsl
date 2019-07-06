
Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);

cbuffer test : register(b1)
{
	float4 time2;
}

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float2 uv: UV;
};

// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DiffuseSampler, input.uv);
	diffuse.x += time2.x;
	return diffuse;
}