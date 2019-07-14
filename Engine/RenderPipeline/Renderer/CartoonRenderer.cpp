#include "pch.h"
#include "CartoonRenderer.h"

Engine::CartoonRenderer::CartoonRenderer(
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<DX::MainLoader>& mainLoader,
	const std::shared_ptr<RenderData>& renderData,
	const std::shared_ptr<GlobalConstantBuffer>& constantData)
	:BaseRenderer(deviceResources, mainLoader, renderData, constantData)
{
	m_OpaquePass = std::shared_ptr<OpaquePass>(new OpaquePass(deviceResources, mainLoader, renderData, constantData, 0)); //´´½¨Pass
}

void Engine::CartoonRenderer::SetPass()
{
	ClearPass();
	m_OpaquePass->SetUp();
	AddPass(m_OpaquePass);
}
