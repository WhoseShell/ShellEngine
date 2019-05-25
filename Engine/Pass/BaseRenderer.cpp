#include "pch.h"
#include "BaseRenderer.h"
//#include "..\Common\DirectXHelper.h"

using namespace DX;
using namespace Engine;

namespace Engine 
{
	bool comp(const std::shared_ptr<BasePass> & a, const std::shared_ptr<BasePass> & b)
	{
		return a->m_passQueue < b->m_passQueue;
	};
}

Engine::BaseRenderer::BaseRenderer(const std::shared_ptr<DX::DeviceResources>& deviceResources, const std::shared_ptr<DX::MainLoader>& mainLoader, const std::shared_ptr<RenderData>& renderData)
	:m_deviceResources(deviceResources), m_mainLoader(mainLoader), m_renderData(renderData) {}

void Engine::BaseRenderer::sort()
{
	if(m_RenderPassList.size() == 0)
	{
		return;
	}

	std::sort(m_RenderPassList.begin(), m_RenderPassList.end(), comp);
}

void Engine::BaseRenderer::ExecuteSequentially()
{	
	if (m_RenderPassList.size() == 0)
	{
		return;
	}
	for (std::vector<std::shared_ptr<BasePass>>::iterator it = m_RenderPassList.begin(); it != m_RenderPassList.end(); it++)
	{
		(*it)->Execute();
	}
}

void Engine::BaseRenderer::FrameReleaseSequentially()
{
	if (m_RenderPassList.size() == 0)
	{
		return;
	}
	for (std::vector<std::shared_ptr<BasePass>>::iterator it = m_RenderPassList.begin(); it != m_RenderPassList.end(); it++)
	{
		(*it)->FrameRelease();
	}
}

void Engine::BaseRenderer::AddPass(const std::shared_ptr<BasePass>& pass)
{
	m_RenderPassList.push_back(pass);
}
