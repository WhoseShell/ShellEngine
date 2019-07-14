#pragma once
#include "..\Common\DeviceResources.h"
#include "ResourceLoad\MainLoader.h"
#include "BasePass.h"

namespace Engine
{
	class OpaquePass : public BasePass
	{
	public:
		OpaquePass(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData,
			const std::shared_ptr<GlobalConstantData>& constantData,
			int passQueue
		);

		void SetUp(); // run once
		void Execute();
		void FrameRelease();
	private:
		D3D11_RASTERIZER_DESC m_rsDesc;
		ComPtr<ID3D11RasterizerState> m_rasterState;
	};
}
