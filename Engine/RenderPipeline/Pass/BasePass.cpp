#include "pch.h"
#include "BasePass.h"
#include "..\Common\DirectXHelper.h"
#include"Common\Type\Material.h"

using namespace Engine;
using namespace DX;
using namespace Microsoft::WRL;

BasePass::BasePass(
	const std::shared_ptr<DX::DeviceResources>& deviceResources,
	const std::shared_ptr<DX::MainLoader>& mainLoader,
	const std::shared_ptr<RenderData>& renderData,
	const std::shared_ptr<GlobalConstantBuffer>& constantData,
	int passQueue)
	:
	m_deviceResources(deviceResources),
	m_mainLoader(mainLoader),
	m_renderData(renderData),
	m_passQueue(passQueue),
	m_ConstantData(constantData)
{}

void Engine::BasePass::Filter(std::wstring passName)
{
	renderObjects.clear();
	//通过ShaderName过滤需要的渲染对象
	//for (size_t i = 0; i < m_renderData->perObject.size(); i++)
	//{
	//	if (m_renderData->perObject.at(i)->shaderName == shaderName)
	//	{
	//		renderObjects.push_back(m_renderData->perObject.at(i));
	//	}
	//}
	std::vector<std::shared_ptr<Object>>::iterator it;
	for (it = m_renderData->perObject.begin(); it != m_renderData->perObject.end(); it++)
	{
		if ((*it)->material->passName == passName)
		{
			renderObjects.push_back((*it));
		}
	}
}

void Engine::BasePass::Filter(std::wstring passName, int renderQueueBegin, int renderQueueEnd)
{
	renderObjects.clear();
	//通过ShaderName + Filter过滤需要的渲染对象
	std::vector<std::shared_ptr<Object>>::iterator it;
	for (it = m_renderData->perObject.begin(); it != m_renderData->perObject.end(); it++)
	{
		if ((*it)->material->passName == passName && (*it)->material->renderQueue >= renderQueueBegin && (*it)->material->renderQueue <= renderQueueEnd)
		{
			renderObjects.push_back((*it));
		}
	}
}

void Engine::BasePass::SetupMatConstantBuffer()
{
	for (std::vector<std::shared_ptr<Object>>::iterator it = renderObjects.begin(); it != renderObjects.end(); it++)
	{
		for (std::vector<std::shared_ptr<MateriaCB>>::iterator mat = (*it)->material->matCBs.begin(); mat != (*it)->material->matCBs.end(); mat++)
		{
			if (!(*mat)->hasCreated)
			{
				CD3D11_BUFFER_DESC constantBufferDesc((*mat)->dataSize, D3D11_BIND_CONSTANT_BUFFER);
				DX::ThrowIfFailed(
					m_deviceResources->GetD3DDevice()->CreateBuffer(
						&constantBufferDesc,
						NULL,
						(*mat)->constantBuffer.GetAddressOf()
					)
				);
				(*mat)->hasCreated = true;
			}
			m_deviceResources->GetD3DDeviceContext()->UpdateSubresource1(
				(*mat)->constantBuffer.Get(),
				0,
				NULL,
				(*mat)->sourceData,
				0,
				0,
				0
			);

		}
	}
}

void Engine::BasePass::SetGlobalConstantBuffer( std::shared_ptr<Object>&  obj)
{
	auto context = m_deviceResources->GetD3DDeviceContext();
	//更新常量缓冲区
	m_ConstantData->mvp.model = obj->GetTransform4x4();
	context->UpdateSubresource1(
		obj->constantBuffer.Get(),
		0,
		NULL,
		&*m_ConstantData,
		0,
		0,
		0
	);

	// 将常量缓冲区发送到图形设备。
	context->VSSetConstantBuffers1(
		0,
		1,
		obj->constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);
	context->PSSetConstantBuffers1(
		0,
		1,
		obj->constantBuffer.GetAddressOf(),
		nullptr,
		nullptr
	);

	for (std::vector<std::shared_ptr<MateriaCB>>::iterator matCB = obj->material->matCBs.begin(); matCB != obj->material->matCBs.end(); matCB++)
	{
		//创建了CB则绑定到vs/ps
		if ((*matCB)->hasCreated)
		{
			context->VSSetConstantBuffers1(
				(*matCB)->startSlot,
				1,
				(*matCB)->constantBuffer.GetAddressOf(),
				nullptr,
				nullptr
			);

			context->PSSetConstantBuffers1(
				(*matCB)->startSlot,
				1,
				(*matCB)->constantBuffer.GetAddressOf(),
				nullptr,
				nullptr
			);
		}
	}

}
