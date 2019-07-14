
Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);

cbuffer test : register(b1)
{
	float4 time2;
}

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR;
	float2 uv: UV;
};

// (�ڲ�)��ɫ���ݵĴ��ݺ�����
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DiffuseSampler, input.uv);
	diffuse.x += time2.x;
	return diffuse;
}