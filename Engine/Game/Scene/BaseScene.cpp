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

std::shared_ptr<Material> Engine::BaseScene::GetMatByName(std::wstring matName)
{
	std::vector<std::shared_ptr<Material>>::iterator it;
	for (it = materialPool.begin(); it != materialPool.end(); it++)
	{
		if ((*it)->name == matName)
		{
			return (*it);
		}
	}
	return nullptr;
}

std::shared_ptr<PerObjectData> Engine::BaseScene::GetObjectByNmae(std::wstring objName)
{
	for (std::vector<std::shared_ptr<PerObjectData>>::iterator it = m_renderData->perObject.begin(); it != m_renderData->perObject.end(); it++)
	{
		if ((*it)->objectName == objName)
		{
			return(*it);
		}
	}
	return nullptr;
}

std::shared_ptr<Material> Engine::BaseScene::CreateMaterial(std::wstring shaderName, std::wstring matName, std::wstring passName, D3D11_CULL_MODE cullMode, int renderQueue)
{
	auto material = std::shared_ptr<Material>(new Material);
	auto shader = m_mainLoader->m_shaderLoader->GetByName(shaderName);
	material->name = matName;
	material->passName = passName;
	material->inputLayout = shader->inputLayout;
	material->vertexShader = shader->vertexShader;
	material->pixelShader = shader->pixelShader;
	material->cullMode = cullMode;
	material->renderQueue = renderQueue;
	return material;
}

void Engine::BaseScene::AssembObject(std::shared_ptr<PerObjectData>& object, std::wstring objectName, std::wstring meshName, std::wstring matName, XMMATRIX & transform, int indexCount)
{
	auto mesh = m_mainLoader->m_meshLoader->GetByName(meshName);
	auto mat = GetMatByName(matName);
	
	object->objectName = objectName;

	object->vertexBuffer = mesh->vertexBuffer;
	object->vertexStride = mesh->vertexStride;
	object->indexBuffer = mesh->indexBuffer;
	if (indexCount <= 0)
	{
		object->indexCount = mesh->indexCount;
	}
	else
	{
		object->indexCount = indexCount;
	}
	object->material = mat;

	object->constantBuffer = constantBuffer;

	//transform
	XMStoreFloat4x4(&object->transform, transform);
}

