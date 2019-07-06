// �洢���ڹ��ɼ���ͼ�ε��������������Ⱦ���ĳ�����������
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;

	float time;
};

cbuffer test : register(b1)
{
	float4 time2;
}

// ����������ɫ�������ÿ����������ݡ�
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
	float2 uv : UV;
};

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
	float2 uv: UV;
};

// ������ GPU ��ִ�ж��㴦��ļ���ɫ����
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);
	//��y����ת
	//matrix rotationY = matrix(
	//	float4(cos(time), 0.0f, -sin(time), 0.0f),
	//	float4(0.0f, 1.0f, 0.0f, 0.0f),
	//	float4(sin(time), 0.0f, cos(time), 0.0f),
	//	float4(0.0f, 0.0f, 0.0f, 1.0f));
	//pos = mul(rotationY, pos);

	// ��������ƶ�
	//matrix xxx = view;
	//xxx._34 += time * 0.1f;

	// ������λ��ת��ΪͶӰ�ռ䡣
	pos = mul(model, pos);
	pos = mul(view, pos);
	pos = mul(projection, pos);
	output.pos = pos;

	// �����޸ĵش�����ɫ��
	output.color = input.color;
	output.uv = input.uv;

	return output;
}
