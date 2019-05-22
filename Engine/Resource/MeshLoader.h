#pragma once
#include <ppltasks.h>
#include "BaseLoader.h"

namespace DX
{
	class MeshLoader : public BaseLoader
	{
	public:
		MeshLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
	protected:
		void LoadMesh(
			_In_ Platform::String^ filename,
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
			_In_opt_ Platform::String^ debugName
		);
	};
}