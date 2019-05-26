#pragma once
#include "..\Pass\BasePass.h"

namespace Engine
{
	class BaseRenderer
	{
	public:
		BaseRenderer(
			const std::shared_ptr<DX::DeviceResources>& deviceResources,
			const std::shared_ptr<DX::MainLoader>& mainLoader,
			const std::shared_ptr<RenderData>& renderData
		);
		virtual void SetPass() = 0;
		void ExecuteSequentially();
		void FrameReleaseSequentially();

	protected:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;
		void AddPass(const std::shared_ptr<BasePass> &pass);

	private:
		std::vector<std::shared_ptr<BasePass>> m_RenderPassList;

		void sort();
	};
}