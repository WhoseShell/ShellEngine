#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Resource\MainLoader.h"
#include "ShaderStruct.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Engine
{
	struct Material
	{
		int renderQueue;
		std::wstring name;
		std::wstring passName;

		D3D11_CULL_MODE cullMode;

		ComPtr<ID3D11InputLayout>	inputLayout;
		ComPtr<ID3D11VertexShader>	vertexShader;
		ComPtr<ID3D11PixelShader>	pixelShader;

		std::vector<ComPtr<ID3D11ShaderResourceView>> SRVs;
	};

	struct PerObjectData
	{
		std::wstring objectName;

		ComPtr<ID3D11Buffer>		vertexBuffer;
		ComPtr<ID3D11Buffer>		indexBuffer;
		ComPtr<ID3D11Buffer>		constantBuffer;
		uint32	indexCount;
		uint32 vertexStride;
		
		XMFLOAT4X4 transform;

		std::shared_ptr<Material> material;
	};

	struct LightData
	{
		XMFLOAT3 marblePosition;
		float marbleRadius;
		float lightStrength;
	};

	struct ConstantData
	{
		std::shared_ptr<MVPConstantBuffer> mvp;
	};

	struct RenderData
	{
		std::vector<std::shared_ptr<PerObjectData>> perObject;
		std::shared_ptr<LightData> lightData;
	};

	class BasePass
	{
	public:
		BasePass(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, 
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData,
			const std::shared_ptr<ConstantData>& constantData,
			int passQueue
		);
		int m_passQueue;
		virtual void Execute() = 0;
		virtual void FrameRelease() = 0;
		void Filter(std::wstring passName);
		void Filter(std::wstring passName, int renderQueueBegin, int renderQueueEnd);
	protected:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;
		std::shared_ptr<ConstantData> m_ConstantData;
		std::vector<std::shared_ptr<PerObjectData>> renderObjects;

	private:

	};
	
}