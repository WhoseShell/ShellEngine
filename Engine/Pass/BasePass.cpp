#include "pch.h"
#include "BasePass.h"
#include "..\Common\DirectXHelper.h"

using namespace Engine;
using namespace DX;
using namespace Microsoft::WRL;

BasePass::BasePass(
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<DX::MainLoader>& mainLoader,
	const std::shared_ptr<RenderData>& renderData,
	int passQueue)
	:
	m_deviceResources(deviceResources),
	m_mainLoader(mainLoader),
	m_renderData(renderData),
	m_passQueue(passQueue){}
