#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Resource\MainLoader.h"

using namespace DirectX;

namespace Engine
{
	struct PerObjectData
	{
		std::wstring objectName;
		std::wstring shaderName;
		int renderQueue;

		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer;
		Microsoft::WRL::ComPtr<ID3D11Buffer>		constantBuffer;
		uint32	indexCount;

		//XMFLOAT4X4 model;
		//XMFLOAT4X4 view;        // view matrix
		//XMFLOAT4X4 projection;  // projection matrix
	};

	struct LightData
	{
		XMFLOAT3 marblePosition;
		float marbleRadius;
		float lightStrength;
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
			int passQueue
		);
		int m_passQueue;
		virtual void Execute() = 0;
		virtual void FrameRelease() = 0;

	protected:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;

	private:

	};
	
}