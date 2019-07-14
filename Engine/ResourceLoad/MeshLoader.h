#pragma once
#include <ppltasks.h>
#include "BaseLoader.h"

namespace DX
{
	struct Mesh
	{
		std::wstring name;

		Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
		uint32 vertexCount;
		uint32 vertexStride;

		Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer;
		uint32 indexCount;
	};

	class MeshLoader : public BaseLoader
	{
	public:
		MeshLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		void LoadMesh(
			_In_ Platform::String^ filename,
			int vertexStride,
			_Out_ ID3D11Buffer** vertexBuffer,
			_Out_ ID3D11Buffer** indexBuffer,
			_Out_opt_ uint32* vertexCount,
			_Out_opt_ uint32* indexCount
		);

		void CreateMesh(
			_In_ byte* meshData,
			_Out_ ID3D11Buffer** vertexBuffer,
			_Out_ ID3D11Buffer** indexBuffer,
			_Out_opt_ uint32* vertexCount,
			_Out_opt_ uint32* indexCount,
			int vertexStride,
			_In_opt_ Platform::String^ debugName
		);

		void LoadMesh(_In_ Platform::String^ filename, Platform::String ^ meshName, int vertexStride);

		std::vector<std::shared_ptr<Mesh>> meshPool;

		std::shared_ptr<Mesh> GetByName(std::wstring name);
	protected:
		
	};
}