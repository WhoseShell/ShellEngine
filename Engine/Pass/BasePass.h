#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Resource\MainLoader.h"
#include "ShaderStruct.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Engine
{
	struct PerObjectData
	{
		std::wstring objectName;
		std::wstring shaderName;
		int renderQueue;

		ComPtr<ID3D11Buffer>		vertexBuffer;
		ComPtr<ID3D11Buffer>		indexBuffer;
		ComPtr<ID3D11Buffer>		constantBuffer;
		uint32	indexCount;
		uint32 vertexStride;

		ComPtr<ID3D11VertexShader>	vertexShader;
		ComPtr<ID3D11PixelShader>	pixelShader;
		ComPtr<ID3D11InputLayout>	inputLayout;
		
		ComPtr<ID3D11ShaderResourceView>		normal;
		ComPtr<ID3D11ShaderResourceView>		baseColor;
		ComPtr<ID3D11ShaderResourceView>		metaAndSmothness;
		
		XMFLOAT4X4 transform;
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
		void Filter(std::wstring shaderName);
		void Filter(std::wstring shaderName, int renderQueueBegin, int renderQueueEnd);
	protected:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;
		std::shared_ptr<ConstantData> m_ConstantData;
		std::vector<std::shared_ptr<PerObjectData>> renderObjects;

	private:

	};
	
}