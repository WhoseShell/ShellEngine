#include "pch.h"
#include "CartoonScene.h"
#include "..\Common\DirectXHelper.h"

using namespace std;
using namespace Windows::Foundation;
using namespace Engine;
using namespace DirectX;

Engine::CartoonScene::CartoonScene(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	:BaseScene(deviceResources),
	m_degreesPerSecond(45)
{
	m_cartoonRenderer = std::shared_ptr<CartoonRenderer>(new CartoonRenderer(deviceResources, m_mainLoader, m_renderData, m_constantData)); //创建Renderer
	m_constantBufferData = std::shared_ptr<MVPConstantBuffer>(new MVPConstantBuffer);
	m_moveController = std::unique_ptr<MoveController>(new MoveController());
}

void Engine::CartoonScene::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}


	XMMATRIX perspectiveMatrix = XMMatrixPerspectiveFovRH(
		fovAngleY,
		aspectRatio,
		0.01f,
		100.0f
	);

	XMFLOAT4X4 orientation = m_deviceResources->GetOrientationTransform3D();

	XMMATRIX orientationMatrix = DirectX::XMLoadFloat4x4(&orientation);

	DirectX::XMStoreFloat4x4(
		&(m_constantBufferData->projection),
		perspectiveMatrix * orientationMatrix
	);

	eye = { 0.0f, 0.0f, -3.0f };
	at = { 0.0f, 0.0f, 1.0f };
	up = { 0.0f, 1.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&(m_constantBufferData->view), XMMatrixLookAtRH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&at), DirectX::XMLoadFloat3(&up)));
}

void Engine::CartoonScene::Init()
{
#pragma region 加载VB/VS

	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer;
	uint32 indexCount;
	uint32 vertexCount;
	uint32 vertexStride;

	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Face.bin", vertexBuffer.GetAddressOf(), indexBuffer.GetAddressOf(), &vertexCount, &indexCount, &vertexStride);

#pragma endregion

#pragma region 创建VS/PS/IA

	D3D11_INPUT_ELEMENT_DESC SCDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R16G16_UNORM, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	std::vector<int> shaderConut;
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", SCDesc, ARRAYSIZE(SCDesc), shaderConut);

#pragma endregion

#pragma region 创建ConstantBuffer / 更新装配ConstantData
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer1;
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer) + 12, D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			constantBuffer1.GetAddressOf()
		)
	);
	CreateWindowSizeDependentResources(); // 更新m_constantBufferData;
	m_constantData->mvp = m_constantBufferData;
#pragma endregion

#pragma region 加载贴图/SRV

	int face_baseColorID = m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\face_BaseColor.dds", L"diffuse");
	int cloth_baseColorID = m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\cloth.dds", L"diffuse");

#pragma endregion

#pragma region 创建/绑定采样器
	// 创建采样描述
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//创建SampleState
	ID3D11SamplerState *   m_sampler;
	m_deviceResources->GetD3DDevice()->CreateSamplerState(
		&sampDesc,
		&m_sampler
	);

	//绑定SampleSate到PS
	m_deviceResources->GetD3DDeviceContext()->PSSetSamplers(
		0,
		1,
		&m_sampler
	);
#pragma endregion

#pragma region 装配PerObject
	for (size_t i = 0; i < 2; i++)
	{
		auto currentObj = std::shared_ptr<PerObjectData>(new PerObjectData);//创建一个渲染对象

		if (i == 0)
		{
			currentObj->objectName = L"Face";
			currentObj->indexCount = 18576;
			currentObj->baseColor = m_mainLoader->m_textureLoader->allSRV[face_baseColorID].SRV;
		}
		if (i == 1)
		{
			m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\cloth.bin", vertexBuffer.GetAddressOf(), indexBuffer.GetAddressOf(), &vertexCount, &indexCount, &vertexStride);
			currentObj->indexCount = 42108;
			currentObj->baseColor = m_mainLoader->m_textureLoader->allSRV[cloth_baseColorID].SRV;
		}

		//设置该对象name及shaderNama
		currentObj->objectName = wstring(L"卡通") + (i.ToString())->Data();
		currentObj->shaderName = wstring(L"OpaquePass");
		currentObj->renderQueue = 2000;

		//Buffer
		currentObj->vertexBuffer = vertexBuffer;
		currentObj->indexBuffer = indexBuffer;
		currentObj->vertexStride = vertexStride;
		currentObj->constantBuffer = constantBuffer1;

		// IA/VS/PS
		currentObj->vertexShader = m_mainLoader->m_shaderLoader->allVertexShader[0];
		currentObj->pixelShader = m_mainLoader->m_shaderLoader->allPixelShader[0];
		currentObj->inputLayout = m_mainLoader->m_shaderLoader->allInputLayout[0];

		//transform
		XMStoreFloat4x4(&currentObj->transform, XMMatrixTranslation(0.0f, 0.0f, 0.0f));

		m_renderData->perObject.push_back(currentObj);//将对象加入对象池
	}
#pragma endregion

}

void Engine::CartoonScene::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		Rotate(0.01f);
		//Rotate(0);
		m_constantBufferData->time = (float)timer.GetTotalSeconds();
	}
	if (m_moveController != nullptr)
	{
		m_moveController->MoveCamera(m_constantBufferData->view, u_state, eye, at, up);
	}
}

void Engine::CartoonScene::Render()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_cartoonRenderer->SetPass(); //设置renderPass

	m_cartoonRenderer->ExecuteSequentially();
}

void Engine::CartoonScene::Release()
{
	m_cartoonRenderer->FrameReleaseSequentially();
	m_renderData->perObject.clear();
}

void Engine::CartoonScene::Rotate(float radians)
{
	for (vector<shared_ptr<PerObjectData>>::iterator it = m_renderData->perObject.begin(); it != m_renderData->perObject.end(); it++)
	{
		XMStoreFloat4x4(&(*it)->transform, XMMatrixMultiply(XMMatrixRotationY(radians), XMLoadFloat4x4(&(*it)->transform)));
	}
}
