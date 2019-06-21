#pragma once
// 用于向顶点着色器发送 MVP 矩阵的常量缓冲区。
struct MVPConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	float time;
};

// 用于设置每个顶点的数据，由mesh的顶点数据结构唯一决定。不一定是VS中得到的数据，还要看InputLyout的定义
struct VertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 uv;
};

