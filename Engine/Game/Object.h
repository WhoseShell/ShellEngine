#pragma once
#include"Material.h"

namespace Engine
{
	class Object
	{
	public:
		std::wstring objectName;

		ComPtr<ID3D11Buffer>		vertexBuffer;
		ComPtr<ID3D11Buffer>		indexBuffer;
		ComPtr<ID3D11Buffer>		constantBuffer;
		uint32	indexCount;
		uint32 vertexStride;

		XMFLOAT4X4 transform;

		std::shared_ptr<Material> material;
	};
}