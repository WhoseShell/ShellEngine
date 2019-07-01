#pragma once
#include"Common\DeviceResources.h"
#include"Common\DirectXHelper.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Engine
{
	class Material
	{
	public:
		int renderQueue;
		std::wstring name;
		std::wstring passName;

		D3D11_CULL_MODE cullMode;

		ComPtr<ID3D11InputLayout>	inputLayout;
		ComPtr<ID3D11VertexShader>	vertexShader;
		ComPtr<ID3D11PixelShader>	pixelShader;

		std::vector<ComPtr<ID3D11ShaderResourceView>> SRVs;

		ComPtr<ID3D11Buffer>		constantBuffer;
	
		void SetTexture(ComPtr<ID3D11ShaderResourceView> & srv);

	
	};
}