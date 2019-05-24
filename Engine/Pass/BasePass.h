#pragma once
#include "..\Common\DeviceResources.h"
#include "..\Resource\MainLoader.h"

namespace Engine
{
	class BasePass
	{
	public:
		BasePass(
			const std::shared_ptr<DX::DeviceResources>& deviceResources, 
			const std::shared_ptr<DX::MainLoader>& mainLoader
		);

	protected:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		
	};
	
}