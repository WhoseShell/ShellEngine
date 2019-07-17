#pragma once
// 用于向顶点着色器发送 MVP 矩阵的常量缓冲区。
struct MVPConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	DirectX::XMFLOAT4 time;

};

struct ScatterConstantBuffer
{
	DirectX::XMFLOAT4		u_CameraPosition;//相机世界坐标位置
	DirectX::XMFLOAT4		u_SunDirection;
	DirectX::XMFLOAT4		u_RayleighColorM20;
	DirectX::XMFLOAT4		u_RayleighColorM10;
	DirectX::XMFLOAT4		u_RayleighColorMP0;
	DirectX::XMFLOAT4		u_RayleighColorP10;
	DirectX::XMFLOAT4		u_RayleighColorP20;

	DirectX::XMFLOAT4		u_MieColorM20;
	DirectX::XMFLOAT4		u_MieColorMP0;
	DirectX::XMFLOAT4		u_MieColorP20;

	float		u_HeightNormalDistanceRcp;
	float		u_HeightNearScatterPush;
	float		u_HeightRayleighDensity ;
	float		u_HeightMieDensity;
	
	float		u_HeightSeaLevel ;
	DirectX::XMFLOAT3		u_HeightPlaneShift;
	
	float		u_HeightDistanceRcp;
	float		u_WorldScaleExponent;
	float		u_WorldNearScatterPush ;
	float		u_WorldRayleighDensity;
	
	float		u_WorldMieDensity ;
	float		u_WorldNormalDistanceRcp;
	DirectX::XMFLOAT2		u_RayleighInScatterPct;
	
	DirectX::XMFLOAT4		u_HeightRayleighColor;

	float		raylieHeightDensity;
	float		u_MiePhaseAnisotropy;
	float		u_MieColorIntensity;
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
