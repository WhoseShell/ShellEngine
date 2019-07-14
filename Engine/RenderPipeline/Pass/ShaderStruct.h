#pragma once
// �����򶥵���ɫ������ MVP ����ĳ�����������
struct MVPConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;

	float time;
};

struct ScatterConstantBuffer
{
	DirectX::XMFLOAT3		u_CameraPosition;//�����������λ��
	DirectX::XMFLOAT3		u_SunDirection;

	float		u_WorldScaleExponent; //������������
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

// ��������ÿ����������ݣ���mesh�Ķ������ݽṹΨһ��������һ����VS�еõ������ݣ���Ҫ��InputLyout�Ķ���
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
