#include "pch.h"
#include "BaseScene.h"

using namespace Engine;

Engine::BaseScene::BaseScene(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::MainLoader>& mainLoader, const std::shared_ptr<RenderData>& renderData)
	:m_deviceResources(deviceResources),
	m_mainLoader(mainLoader),
	m_renderData(renderData){}

