#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Resource\MainLoader.h"
#include "ShaderStruct.h"
#include"Game\Object.h"

using namespace DirectX;
using namespace Microsoft::WRL;

namespace Engine
{
	struct LightData
	{
		XMFLOAT3 marblePosition;
		float marbleRadius;
		float lightStrength;
	};

	struct GlobalConstantData
	{
		std::shared_ptr<MVPConstantBuffer> mvp;
	};

	struct RenderData
	{
		std::vector<std::shared_ptr<Object>> perObject;
		std::shared_ptr<LightData> lightData;
	};

	class BasePass
	{
	public:
		BasePass(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, 
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData,
			const std::shared_ptr<GlobalConstantData>& constantData,
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
		std::shared_ptr<GlobalConstantData> m_ConstantData;
		std::vector<std::shared_ptr<Object>> renderObjects;

	private:

	};
	
}