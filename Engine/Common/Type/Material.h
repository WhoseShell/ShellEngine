#pragma once
#include"Common\DeviceResources.h"
#include"Common\DirectXHelper.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Engine
{
	struct MateriaCB
	{
		ComPtr<ID3D11Buffer>		constantBuffer;
		void* sourceData;
		int32 dataSize;
		std::wstring name;
		bool hasCreated;
		int startSlot;
	};
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

		std::vector<std::shared_ptr<MateriaCB>>	matCBs;
	
		void SetTexture(ComPtr<ID3D11ShaderResourceView> & srv);

		/* 新加入创建一个，否则更新参数 */
		void SetConstantBuffer(std::wstring name, void* sourceData, int32 dataSize, UINT startSlot);
	};
}