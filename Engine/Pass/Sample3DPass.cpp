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
	int passQueue)
	:
	BasePass(deviceResources, mainLoader, renderData, passQueue){}

void Engine::Sample3DPass::SetUp()
{

}

void Engine::Sample3DPass::Execute()
{
	// ÿ�����㶼�� VertexPositionColor �ṹ��һ��ʵ����
	auto context = m_deviceResources->GetD3DDeviceContext();
	auto device = m_deviceResources->GetD3DDevice();

	// Reset render targets to the screen.
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// Clear the back buffer and depth stencil view.
	context->ClearRenderTargetView(m_deviceResources->GetBackBufferRenderTargetView(), DirectX::Colors::White);
	context->ClearDepthStencilView(m_deviceResources->GetDepthStencilView(), D3D11_CLEAR_DEPTH | D3D11_CLEAR_STENCIL, 1.0f, 0);


	UINT stride = sizeof(VertexPosColor);
	UINT offset = 0;
	context->IASetVertexBuffers(
		0,
		1,
		m_renderData->perObject.at(0)->vertexBuffer.GetAddressOf(),
		&stride,
		&offset
	);

	context->IASetIndexBuffer(
		m_renderData->perObject.at(0)->indexBuffer.Get(),
		DXGI_FORMAT_R16_UINT, // ÿ����������һ�� 16 λ�޷�������(short)��``
		0
	);

	context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

	context->IASetInputLayout(m_renderData->perObject.at(0)->inputLayout.Get());

	// �������ǵĶ�����ɫ����
	context->VSSetShader(
		m_renderData->perObject.at(0)->vertexShader.Get(),
		nullptr,
		0
	);

	// ���������������͵�ͼ���豸��
	context->VSSetConstantBuffers1(
		0,
		1,
		m_renderData->perObject.at(0)->constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	// �������ǵ�������ɫ����
	context->PSSetShader(
		m_renderData->perObject.at(0)->pixelShader.Get(),
		nullptr,
		0
	);

	// ��SRV
	context->PSSetShaderResources(0, 1, m_renderData->perObject.at(0)->baseColor.GetAddressOf());


	// ���ƶ���
	context->DrawIndexed(
		m_renderData->perObject.at(0)->indexCount,
		0,
		0
	);
}

void Engine::Sample3DPass::FrameRelease()
{

}

