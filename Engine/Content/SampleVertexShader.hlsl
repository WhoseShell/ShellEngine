// �洢���ڹ��ɼ���ͼ�ε��������������Ⱦ���ĳ�����������
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;

	float time;
};

// ����������ɫ�������ÿ����������ݡ�
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};

// ͨ��������ɫ�����ݵ�ÿ�����ص���ɫ���ݡ�
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
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

	return output;
}
