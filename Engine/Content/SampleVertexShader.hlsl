// 存储用于构成几何图形的三个基本列优先矩阵的常量缓冲区。
cbuffer ModelViewProjectionConstantBuffer : register(b0)
{
	matrix model;
	matrix view;
	matrix projection;

	float time;
};

// 用作顶点着色器输入的每个顶点的数据。
struct VertexShaderInput
{
	float3 pos : POSITION;
	float3 color : COLOR0;
};

// 通过像素着色器传递的每个像素的颜色数据。
struct PixelShaderInput
{
	float4 pos : SV_POSITION;
	float3 color : COLOR0;
};

// 用于在 GPU 上执行顶点处理的简单着色器。
PixelShaderInput main(VertexShaderInput input)
{
	PixelShaderInput output;
	float4 pos = float4(input.pos, 1.0f);

	//绕y轴旋转
	//matrix rotationY = matrix(
	//	float4(cos(time), 0.0f, -sin(time), 0.0f),
	//	float4(0.0f, 1.0f, 0.0f, 0.0f),
	//	float4(sin(time), 0.0f, cos(time), 0.0f),
	//	float4(0.0f, 0.0f, 0.0f, 1.0f));
	//pos = mul(rotationY, pos);

	// 靠近相机移动
	//matrix xxx = view;
	//xxx._34 += time * 0.1f;

	// 将顶点位置转换为投影空间。
	pos = mul(model, pos);
	pos = mul(view, pos);
	pos = mul(projection, pos);
	output.pos = pos;

	// 不加修改地传递颜色。
	output.color = input.color;

	return output;
}
