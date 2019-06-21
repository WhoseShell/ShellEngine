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

	// ����������Ϊ���������Ļ��
	auto viewport = m_deviceResources->GetScreenViewport();
	context->RSSetViewports(1, &viewport);

	// ������Ŀ������Ϊ��Ļ��
	ID3D11RenderTargetView *const targets[1] = { m_deviceResources->GetBackBufferRenderTargetView() };
	context->OMSetRenderTargets(1, targets, m_deviceResources->GetDepthStencilView());

	// �����̨�����������ģ����ͼ��
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
			DXGI_FORMAT_R16_UINT, // ÿ����������һ�� 16 λ�޷�������(short)��``
			0
		);

		context->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);

		context->IASetInputLayout((*it)->inputLayout.Get());

		// �������ǵĶ�����ɫ����
		context->VSSetShader(
			(*it)->vertexShader.Get(),
			nullptr,
			0
		);

		//���³���������
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

		// ���������������͵�ͼ���豸��
		context->VSSetConstantBuffers1(
			0,
			1,
			(*it)->constantBuffer.GetAddressOf(),
			nullptr,
			nullptr
		);

		// �������ǵ�������ɫ����
		context->PSSetShader(
			(*it)->pixelShader.Get(),
			nullptr,
			0
		);

		// ��SRV
		context->PSSetShaderResources(0, 1, (*it)->baseColor.GetAddressOf());


		// ���ƶ���
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

