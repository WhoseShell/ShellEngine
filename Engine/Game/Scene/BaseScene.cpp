#include "pch.h"
#include "BaseScene.h"

using namespace Engine;
using namespace DX;

Engine::BaseScene::BaseScene(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	:m_deviceResources(deviceResources)
{
	m_mainLoader = std::shared_ptr<MainLoader>(new MainLoader(deviceResources->GetD3DDevice()));
	m_renderData = std::shared_ptr<RenderData>(new RenderData);
	m_globalConstantData = std::shared_ptr<GlobalConstantBuffer>(new GlobalConstantBuffer);
	m_MVPConstantData = std::shared_ptr<MVPConstantBuffer>(&m_globalConstantData->mvp);
	m_scatterConstantData = std::shared_ptr<ScatterConstantBuffer>(&m_globalConstantData->scatter);
	CreateGlobalConstantBuffer();
}

void Engine::BaseScene::InputUserState(UserState^ ustate)
{
	m_userState = ustate;
}

std::shared_ptr<Material> Engine::BaseScene::GetMatByName(std::wstring matName)
{
	std::vector<std::shared_ptr<Material>>::iterator it;
	for (it = m_materialPool.begin(); it != m_materialPool.end(); it++)
	{
		if ((*it)->name == matName)
		{
			return (*it);
		}
	}
	return nullptr;
}

std::shared_ptr<Object> Engine::BaseScene::GetObjectByNmae(std::wstring objName)
{
	for (std::vector<std::shared_ptr<Object>>::iterator it = m_renderData->perObject.begin(); it != m_renderData->perObject.end(); it++)
	{
		if ((*it)->objectName == objName)
		{
			return(*it);
		}
	}
	return nullptr;
}

std::shared_ptr<Material> Engine::BaseScene::CreateMaterial(std::wstring matName, std::wstring shaderName, std::wstring passName, D3D11_CULL_MODE cullMode, int renderQueue)
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

void Engine::BaseScene::AssembObject(std::shared_ptr<Object>& object, std::wstring objectName, std::wstring meshName, std::wstring matName, std::shared_ptr<Transform> transform, int indexCount)
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

	object->constantBuffer = m_constantBuffer;

	//transform
	object->transform = transform;
}

void Engine::BaseScene::ResetScatterProperty()
{
	m_scatterConstantData->u_CameraPosition = DirectX::XMFLOAT4(eye.x, eye.y, eye.z, 1.0f);
	m_scatterConstantData->u_SunDirection = DirectX::XMFLOAT4(-0.2f, 0.0f, -0.5f, 1.0f);
	m_scatterConstantData->u_RayleighColorM20 = DirectX::XMFLOAT4(0.758f, 0.612f, 0.377f, 1.0f);
	m_scatterConstantData->u_RayleighColorM20 = DirectX::XMFLOAT4(0.758f, 0.612f, 0.377f, 1.0f);
	m_scatterConstantData->u_RayleighColorM10 = DirectX::XMFLOAT4(0.758f, 0.612f, 0.377f, 1.0f);
	m_scatterConstantData->u_RayleighColorMP0 = DirectX::XMFLOAT4(0.758f, 0.612f, 0.377f, 1.0f);
	m_scatterConstantData->u_RayleighColorP10 = DirectX::XMFLOAT4(1.0f, 0.63f, 0.17f, 1.0f);
	m_scatterConstantData->u_RayleighColorP20 = DirectX::XMFLOAT4(0.758f, 0.612f, 0.377f, 1.0f);

	m_scatterConstantData->u_MieColorM20 = DirectX::XMFLOAT4(1.0f, 0.612f, 0.13f, 1.0f);
	m_scatterConstantData->u_MieColorMP0 = DirectX::XMFLOAT4(1.0f, 0.64f, 0.13f, 1.0f);
	m_scatterConstantData->u_MieColorP20 = DirectX::XMFLOAT4(1.0f, 0.612f, 0.12f, 1.0f);

	m_scatterConstantData->u_HeightNormalDistanceRcp = 1.0f / 1000.0f;;
	m_scatterConstantData->u_HeightNearScatterPush = 0;
	m_scatterConstantData->u_HeightRayleighDensity = -0 / 100000.0f;
	m_scatterConstantData->u_HeightMieDensity = 0;
	m_scatterConstantData->u_HeightSeaLevel = 0;
	m_scatterConstantData->u_HeightPlaneShift = DirectX::XMFLOAT3(0.0f, 0.0f, 0.0f);
	m_scatterConstantData->u_HeightDistanceRcp = 1.0f / 50.0f;

	m_scatterConstantData->u_WorldScaleExponent = 1.0f;
	m_scatterConstantData->u_WorldNearScatterPush = 0.0f;
	m_scatterConstantData->u_WorldRayleighDensity = -2000.0f / 100000.0f;
	m_scatterConstantData->u_WorldMieDensity = -200.9f / 100000.0f;
	m_scatterConstantData->u_WorldNormalDistanceRcp = 1.0f / 1000.0f;
	m_scatterConstantData->raylieHeightDensity = 1.23f;

	m_scatterConstantData->u_RayleighInScatterPct = DirectX::XMFLOAT2(0.4f, 0.6f);

	m_scatterConstantData->u_MiePhaseAnisotropy = 0.8f;
	m_scatterConstantData->u_MieColorIntensity = 2.0f;
	m_scatterConstantData->u_HeightRayleighColor = DirectX::XMFLOAT4(0.97f, 0.97f, 0.97f, 1.0f);
}

void Engine::BaseScene::CreateGlobalConstantBuffer()
{
	int bufferSize = 0;
	for (int i = 0; i < 1000; i++)
	{
		if (i * 16 >= sizeof(GlobalConstantBuffer))
		{
			bufferSize = i * 16;
			break;
		}
	}
	CD3D11_BUFFER_DESC constantBufferDesc(bufferSize, D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			m_constantBuffer.GetAddressOf()
		)
	);
}


