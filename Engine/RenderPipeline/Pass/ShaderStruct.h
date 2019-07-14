#pragma once
// 用于向顶点着色器发送 MVP 矩阵的常量缓冲区。
struct MVPConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	float time;
};

struct ScatterConstantBuffer
{
	DirectX::XMFLOAT3		u_CameraPosition;//相机世界坐标位置
	DirectX::XMFLOAT3		u_SunDirection;

	float		u_WorldScaleExponent; //世界坐标缩放
	float		u_WorldNearScatterPush;
	float		u_WorldRayleighDensity;
	float		u_WorldMieDensity;
	float		raylieHeightDensity;

	DirectX::XMFLOAT3		u_RayleighColorM20;
	DirectX::XMFLOAT3		u_RayleighColorM10;
	DirectX::XMFLOAT3		u_RayleighColorO00;
	DirectX::XMFLOAT3		u_RayleighColorP10;
	DirectX::XMFLOAT3		u_RayleighColorP20;
	DirectX::XMFLOAT3		u_RayleighColorP45;

	DirectX::XMFLOAT3		u_MieColorM20;
	DirectX::XMFLOAT3		u_MieColorO00;
	DirectX::XMFLOAT3		u_MieColorP20;
	DirectX::XMFLOAT3		u_MieColorP45;

	float		u_MieColorIntensity;
	float		u_MiePhaseAnisotropy;
	float		null;
};

struct GlobalConstantBuffer
{
	MVPConstantBuffer mvp;
	ScatterConstantBuffer scatter;
};

// 用于设置每个顶点的数据，由mesh的顶点数据结构唯一决定。不一定是VS中得到的数据，还要看InputLyout的定义
struct VertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
	DirectX::XMFLOAT2 uv;
};

struct TestConstantBuffer
{
	DirectX::XMFLOAT4 test;
};
