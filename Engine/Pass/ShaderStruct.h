#pragma once
// �����򶥵���ɫ������ MVP ����ĳ�����������
struct MVPConstantBuffer
{
	DirectX::XMFLOAT4X4 model;
	DirectX::XMFLOAT4X4 view;
	DirectX::XMFLOAT4X4 projection;
};

// �����򶥵���ɫ������ÿ����������ݡ�
struct VertexPosColor
{
	DirectX::XMFLOAT3 pos;
	DirectX::XMFLOAT3 color;
};