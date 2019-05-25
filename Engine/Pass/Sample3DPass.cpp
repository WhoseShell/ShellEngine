#include "pch.h"
#include "Sample3DPass.h"
#include "..\Common\DirectXHelper.h"


using namespace DX;
using namespace Microsoft::WRL;
using namespace Engine;

Engine::Sample3DPass::Sample3DPass(
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::shared_ptr<DX::MainLoader>& mainLoader,
	const std::shared_ptr<RenderData>& renderData,
	int passQueue)
	:
	BasePass(deviceResources, mainLoader, renderData, passQueue){}

void Engine::Sample3DPass::SetUp()
{

}

void Engine::Sample3DPass::Execute()
{

}

void Engine::Sample3DPass::FrameRelease()
{

}

