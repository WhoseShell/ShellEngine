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
		0.05f,
		5000.0f
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
#pragma region 装配PerObject
	for (size_t i = 0; i < 3; i++)
	{
		auto currentObj = std::shared_ptr<Object>(new Object);//创建一个渲染对象

		if (i == 0)
		{
			XMMATRIX transform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
			AssembObject(currentObj, L"face", L"face", L"face", transform, 18756);
		}
		if (i == 1)
		{
			XMMATRIX transform = XMMatrixScaling(1.0f, 1.0f, 1.0f);
			AssembObject(currentObj, L"cloth", L"cloth", L"cloth", transform, 42108);
		}
		if (i == 2)
		{
			XMMATRIX transform = XMMatrixScaling(3000.0f, 3000.0f, 3000.0f);
			AssembObject(currentObj, L"skyBox", L"skyBox", L"skyBox", transform, -1);
		}

		m_renderData->perObject.push_back(currentObj);//将对象加入对象池
	}
#pragma endregion
}

void Engine::CartoonScene::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		//Rotate(0.01f);
		//Rotate(0);
		m_constantBufferData->time = (float)timer.GetTotalSeconds();
	}
	if (m_moveController != nullptr)
	{
		m_moveController->MoveCamera(m_constantBufferData->view, u_state, eye, at, up);
	}

	//skybox位置跟随相机
	auto skybox = GetObjectByNmae(L"skyBox");
	if (skybox != nullptr)
	{
		skybox->transform._41 = eye.x;
		skybox->transform._42 = eye.y;
		skybox->transform._43 = eye.z;
	}
}

void Engine::CartoonScene::Render()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_cartoonRenderer->SetPass(); //设置renderPass

	m_cartoonRenderer->ExecuteSequentially();
}

void Engine::CartoonScene::LoadResource()
{
#pragma region 加载Mesh

	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Face.bin", L"face", 60);
	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\cloth.bin", L"cloth", 60);
	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Sphere.bin", L"skyBox", 56);

#pragma endregion

#pragma region VS/PS/IA

	D3D11_INPUT_ELEMENT_DESC SCDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R16G16_UNORM, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC SphereDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R8G8B8A8_UNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R16G16_UNORM, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", SCDesc, ARRAYSIZE(SCDesc), L"diffuse");
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", SphereDesc, ARRAYSIZE(SphereDesc), L"sphereDiffuse");

#pragma endregion

#pragma region 创建ConstantBuffer / 更新装配ConstantData

	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer) + 12, D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			constantBuffer.GetAddressOf()
		)
	);
	CreateWindowSizeDependentResources(); // 更新m_constantBufferData;
	m_constantData->mvp = m_constantBufferData;
#pragma endregion

#pragma region 加载贴图/shaderResourceView

	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\face_BaseColor.dds", L"face_BaseColor");
	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\cloth.dds", L"cloth_BaseColor");
	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\floor_section1.dds", L"skyBox_BaseColor");

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

#pragma region 创建Material加入材质池
	auto faceMat = CreateMaterial(L"diffuse", L"face", L"OpaquePass", D3D11_CULL_FRONT, 2000);
	faceMat->SRVs.push_back(m_mainLoader->m_textureLoader->GetByName(L"face_BaseColor")->shaderResourceView);
	materialPool.push_back(faceMat);

	auto clothMat = CreateMaterial(L"diffuse", L"cloth", L"OpaquePass", D3D11_CULL_FRONT, 2000);
	clothMat->SRVs.push_back(m_mainLoader->m_textureLoader->GetByName(L"cloth_BaseColor")->shaderResourceView);
	materialPool.push_back(clothMat);

	auto skyBoxMat = CreateMaterial(L"sphereDiffuse", L"skyBox", L"OpaquePass", D3D11_CULL_BACK, 2100);
	skyBoxMat->SRVs.push_back(m_mainLoader->m_textureLoader->GetByName(L"skyBox_BaseColor")->shaderResourceView);
	materialPool.push_back(skyBoxMat);
#pragma endregion

}

void Engine::CartoonScene::Release()
{
	m_cartoonRenderer->FrameReleaseSequentially();
	m_renderData->perObject.clear();
}

void Engine::CartoonScene::Rotate(float radians)
{
	for (vector<shared_ptr<Object>>::iterator it = m_renderData->perObject.begin(); it != m_renderData->perObject.end(); it++)
	{
		XMStoreFloat4x4(&(*it)->transform, XMMatrixMultiply(XMMatrixRotationY(radians), XMLoadFloat4x4(&(*it)->transform)));
	}
}
