#pragma once
#include "..\Resource\MainLoader.h"
#include "Pass\BasePass.h"

namespace Engine
{
	class BaseScene
	{
	public:
		BaseScene(
			const std::shared_ptr<DX::DeviceResources>& deviceResources
		);

	protected:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;

	};
}