
Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
	float2 uv: UV;
};

// (内插)颜色数据的传递函数。
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DiffuseSampler, input.uv);
	return diffuse;
}