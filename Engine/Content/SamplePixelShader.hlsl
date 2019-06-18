
Texture2D DiffuseTexture : register(t0);
SamplerState DiffuseSampler : register(s0);

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
	float2 uv: UV;
};

// (�ڲ�)��ɫ���ݵĴ��ݺ�����
float4 main(PixelShaderInput input) : SV_TARGET
{
	float4 diffuse = DiffuseTexture.Sample(DiffuseSampler, input.uv);
	return diffuse;
}