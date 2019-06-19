#pragma once
#include "BaseRenderer.h"
#include "Pass\Sample3DPass.h"

namespace Engine
{
	class SampleRenderer:public BaseRenderer
	{
	public:
		SampleRenderer(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData,
			const std::shared_ptr<ConstantData>& constantData
		);
		void SetPass();
	private:
		std::shared_ptr<Sample3DPass> m_Sample3DPass;
	};
}