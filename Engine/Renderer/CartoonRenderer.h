#pragma once
#include "BaseRenderer.h"
#include "Pass\OpaquePass.h"

namespace Engine
{
	class CartoonRenderer :public BaseRenderer
	{
	public:
		CartoonRenderer(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData,
			const std::shared_ptr<GlobalConstantData>& constantData
		);
		void SetPass();
	private:
		std::shared_ptr<OpaquePass> m_OpaquePass;
	};
}