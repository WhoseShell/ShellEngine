#include "pch.h"
#include "BaseScene.h"

using namespace Engine;
using namespace DX;

Engine::BaseScene::BaseScene(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	:m_deviceResources(deviceResources)
{
	m_mainLoader = std::shared_ptr<MainLoader>(new MainLoader(deviceResources->GetD3DDevice()));
	m_renderData = std::shared_ptr<RenderData>(new RenderData);
	m_constantData = std::shared_ptr<ConstantData>(new ConstantData);
}

void Engine::BaseScene::InputUserState(UserState^ ustate)
{
	u_state = ustate;
}

