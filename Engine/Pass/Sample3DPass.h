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
			int passQueue
		);

		void SetUp();
		void Execute();
		void FrameRelease();
	};
}
