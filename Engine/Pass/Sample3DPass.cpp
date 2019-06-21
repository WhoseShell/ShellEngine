#include "pch.h"
#include "Sample3DPass.h"
#include "..\Common\DirectXHelper.h"
#include "ShaderStruct.h"


using namespace DX;
using namespace Microsoft::WRL;
using namespace Engine;

Engine::Sample3DPass::Sample3DPass(
	const std::shared_ptr<DX::DeviceResources>& deviceResources, 
	const std::shared_ptr<DX::MainLoader>& mainLoader,
	const std::shared_ptr<RenderData>& renderData,
	const std::shared_ptr<ConstantData>& constantData,
	int passQueue)
	:
	BasePass(deviceResources, mainLoader, renderData, constantData, passQueue){}

void Engine::Sample3DPass::SetUp()
{
	Filter(L"Sample3DPass", 0, 2999);
}

void Engine::Sample3DPass::Execute()
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto device = m_deviceResources->GetD3DDevice();

	// 将视区重置为针对整个屏幕。
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// 将呈现目标重置为屏幕。
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// 清除后台缓冲区和深度模具视图。
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::CornflowerBlue);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	std::vector<std::shared_ptr<PerObjectData>>::iterator it;
	for (it = renderObjects.begin(); it != renderObjects.end(); it++)
	{
		UINT stride = (*it)->vertexStride;
		UINT offset = 0;
		context->IASetVertexBuffers(
			0,
			1,
			(*it)->vertexBuffer.GetAddressOf(),
			&stride,
			&offset
		);

		context->IASetIndexBuffer(
			(*it)->indexBuffer.Get(),
			DXGI_FORMAT_R16_UINT, // 每个索引都是一个 16 位无符号整数(short)。``
			0
		);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->IASetInputLayout((*it)->inputLayout.Get());

		// 附加我们的顶点着色器。
		context->VSSetShader(
			(*it)->vertexShader.Get(),
			nullptr,
			0
		);

		//更新常量缓冲区
		m_ConstantData->mvp->model = (*it)->transform;
		context->UpdateSubresource1(
			(*it)->constantBuffer.Get(),
			0,
			NULL,
			&*m_ConstantData->mvp,
			0,
			0,
			0
		);

		// 将常量缓冲区发送到图形设备。
		context->VSSetConstantBuffers1(
			0,
			1,
			(*it)->constantBuffer.GetAddressOf(),
			nullptr,
			nullptr
		);

		// 附加我们的像素着色器。
		context->PSSetShader(
			(*it)->pixelShader.Get(),
			nullptr,
			0
		);

		// 绑定SRV
		context->PSSetShaderResources(0, 1, (*it)->baseColor.GetAddressOf());


		// 绘制对象。
		context->DrawIndexed(
			(*it)->indexCount,
			0,
			0
		);
	}
}

void Engine::Sample3DPass::FrameRelease()
{

}

