#include "pch.h"
#include "SampleRenderer.h"

Engine::SampleRenderer::SampleRenderer(
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::shared_ptr<DX::MainLoader>& mainLoader, 
	const std::shared_ptr<RenderData>& renderData, 
	const std::shared_ptr<ConstantData>& constantData)
	:BaseRenderer(deviceResources, mainLoader, renderData, constantData)
{
	m_Sample3DPass = std::shared_ptr<Sample3DPass>(new Sample3DPass(deviceResources, mainLoader, renderData, constantData, 0)); //´´½¨Pass
}

void Engine::SampleRenderer::SetPass()
{
	ClearPass();
	m_Sample3DPass->SetUp();
	AddPass(m_Sample3DPass);
}
