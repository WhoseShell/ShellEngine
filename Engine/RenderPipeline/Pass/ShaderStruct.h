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
