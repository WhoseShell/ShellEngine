#include "pch.h"
#include "Sample3DPass.h"
#include "..\Common\DirectXHelper.h"


using namespace DX;
using namespace Microsoft::WRL;

Engine::Sample3DPass::Sample3DPass(
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::shared_ptr<DX::MainLoader>& mainLoader)
	:
	BasePass(deviceResources, mainLoader){}

