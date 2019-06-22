#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Common\DirectXHelper.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Engine
{
	class Material
	{
	public:
		std::wstring name;
		D3D11_CULL_MODE cullMode;

		ComPtr<ID3D11VertexShader>	vertexShader;
		ComPtr<ID3D11PixelShader>	pixelShader;

		std::vector<ComPtr<ID3D11ShaderResourceView>> SRVs;

		void SetSRV(ComPtr<ID3D11ShaderResourceView>);
	};
}