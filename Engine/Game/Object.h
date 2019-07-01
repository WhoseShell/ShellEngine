#pragma once
#include"Material.h"

namespace Engine
{
	struct Transform
	{
		XMFLOAT3 location;
		XMFLOAT3 rotation;
		XMFLOAT3 scale;

		Transform()
		{
			location = XMFLOAT3(0.0f, 0.0f, 0.0f);
			rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
			scale = XMFLOAT3(1.0f, 1.0f, 1.0f);
		};
		Transform(
			FLOAT loc_x, FLOAT loc_y, FLOAT loc_z,
			FLOAT rot_x, FLOAT rot_y, FLOAT rot_z,
			FLOAT sca_x, FLOAT sca_y, FLOAT sca_z)
		{
			location = XMFLOAT3(loc_x, loc_y, loc_z);
			rotation = XMFLOAT3(rot_x, rot_y, rot_z);
			scale = XMFLOAT3(sca_x, sca_y, sca_z);
		};
	};
	class Object
	{
	public:
		std::wstring objectName;

		ComPtr<ID3D11Buffer>		vertexBuffer;
		ComPtr<ID3D11Buffer>		indexBuffer;
		ComPtr<ID3D11Buffer>		constantBuffer;
		uint32	indexCount;
		uint32 vertexStride;

		std::shared_ptr<Transform> transform;

		std::shared_ptr<Material> material;

		XMFLOAT4X4 GetTransform4x4();

		void SetLocation(FLOAT x, FLOAT y, FLOAT z);
		void SetRotation(FLOAT x, FLOAT y, FLOAT z);
		void SetScale(FLOAT x, FLOAT y, FLOAT z);
	};
}