#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Resource\MainLoader.h"
#include "BasePass.h"

namespace Engine
{
	class Sample3DPass : public BasePass
	{
	public:
		Sample3DPass(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData, 
			const std::shared_ptr<ConstantData>& constantData,
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
