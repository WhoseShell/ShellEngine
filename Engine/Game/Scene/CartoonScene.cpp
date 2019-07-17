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
	m_cartoonRenderer = std::shared_ptr<CartoonRenderer>(new CartoonRenderer(deviceResources, m_mainLoader, m_renderData, m_globalConstantData)); //����Renderer
	m_testCB0 = std::shared_ptr<TestConstantBuffer>(new TestConstantBuffer);
	m_testCB1 = std::shared_ptr<TestConstantBuffer>(new TestConstantBuffer);
	m_testCB2 = std::shared_ptr<TestConstantBuffer>(new TestConstantBuffer);
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
		&(m_MVPConstantData->projection),
		perspectiveMatrix * orientationMatrix
	);

	eye = { 0.0f, 0.0f, -3.0f };
	at = { 0.0f, 0.0f, 1.0f };
	up = { 0.0f, 1.0f, 0.0f };

	DirectX::XMStoreFloat4x4(&(m_MVPConstantData->view), XMMatrixLookAtRH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&at), DirectX::XMLoadFloat3(&up)));
}

void Engine::CartoonScene::Init()
{
	CreateWindowSizeDependentResources(); // ����m_constantBufferData;

#pragma region װ��Object
	for (size_t i = 0; i < 3; i++)
	{
		auto currentObj = std::shared_ptr<Object>(new Object);//����һ����Ⱦ����

		if (i == 0)
		{
			shared_ptr<Transform> transform = shared_ptr <Transform>(new Transform);
			AssembObject(currentObj, L"face", L"face", L"face", transform, 18756);
		}
		if (i == 1)
		{
			shared_ptr<Transform> transform = shared_ptr <Transform>(new Transform);
			AssembObject(currentObj, L"cloth", L"cloth", L"cloth", transform, 42108);
		}
		if (i == 2)
		{
			shared_ptr<Transform> transform = shared_ptr <Transform>(new Transform);
			AssembObject(currentObj, L"skyBox", L"skyBox", L"skyBox", transform, -1);
			currentObj->SetScale(3000.0f, 3000.0f, 3000.0f);
		}

		m_renderData->perObject.push_back(currentObj);//�������������
	}
#pragma endregion
}

void Engine::CartoonScene::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		Rotate(0.01f);
		//Rotate(0);
		m_MVPConstantData->time.x = (float)timer.GetTotalSeconds();
	}
	if (m_moveController != nullptr)
	{
		m_moveController->MoveCamera(m_MVPConstantData->view, m_userState, eye, at, up);
	}

	//skyboxλ�ø������
	auto skybox = GetObjectByNmae(L"skyBox");
	if (skybox != nullptr)
	{
		skybox->SetLocation(eye.x, eye.y, eye.z);
	}

	m_testCB0->test.x = 0.0f;
	m_testCB1->test.x = 0.0f;
	m_testCB2->test.x = 0.0f;

	ResetScatterProperty();
}

void Engine::CartoonScene::Render()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_cartoonRenderer->SetPass(); //����renderPass

	m_cartoonRenderer->ExecuteSequentially();
}

void Engine::CartoonScene::LoadResource()
{
#pragma region ����Mesh

	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Mesh\\Face.bin", L"face", 60);
	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Mesh\\cloth.bin", L"cloth", 60);
	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Mesh\\Sphere.bin", L"skyBox", 56);

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

	D3D11_INPUT_ELEMENT_DESC CartoonDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "NORMAL", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TANGENT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 20, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "BLENDWEIGHT", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 28, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "BLENDINDICES", 0, DXGI_FORMAT_R16G16B16A16_SNORM, 0, 44, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "COLOR", 0, DXGI_FORMAT_R16G16_UNORM, 0, 52, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "TEXCOORD", 0, DXGI_FORMAT_R16G16_UNORM, 0, 56, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};


	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", SCDesc, ARRAYSIZE(SCDesc), L"diffuse");
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"ScatterSkyVS.cso", L"ScatterSkyPS.cso", SphereDesc, ARRAYSIZE(SphereDesc), L"skyBox");
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"CartoonVS.cso", L"CartoonPS.cso", CartoonDesc, ARRAYSIZE(CartoonDesc), L"cartoon");

#pragma endregion

#pragma region ������ͼ/shaderResourceView

	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\Texture\\face_BaseColor.dds", L"face_BaseColor");
	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\Texture\\cloth.dds", L"cloth_BaseColor");
	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\Texture\\floor_section1.dds", L"skyBox_BaseColor");

#pragma endregion

#pragma region ����/�󶨲�����
	// ������������
	D3D11_SAMPLER_DESC sampDesc;
	ZeroMemory(&sampDesc, sizeof(sampDesc));
	sampDesc.Filter = D3D11_FILTER_MIN_MAG_MIP_LINEAR;
	sampDesc.AddressU = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressV = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.AddressW = D3D11_TEXTURE_ADDRESS_WRAP;
	sampDesc.ComparisonFunc = D3D11_COMPARISON_NEVER;
	sampDesc.MinLOD = 0;
	sampDesc.MaxLOD = D3D11_FLOAT32_MAX;

	//����SampleState
	ID3D11SamplerState *   m_sampler;
	m_deviceResources->GetD3DDevice()->CreateSamplerState(
		&sampDesc,
		&m_sampler
	);

	//��SampleSate��PS
	m_deviceResources->GetD3DDeviceContext()->PSSetSamplers(
		0,
		1,
		&m_sampler
	);
#pragma endregion

#pragma region ����Material������ʳ�
	auto faceMat = CreateMaterial(L"face", L"cartoon", L"OpaquePass", D3D11_CULL_FRONT, 2000);
	faceMat->SetTexture(m_mainLoader->m_textureLoader->GetByName(L"face_BaseColor")->shaderResourceView);
	faceMat->SetConstantBuffer(L"Test", &*m_testCB0, sizeof(TestConstantBuffer), 1);
	m_materialPool.push_back(faceMat);

	auto clothMat = CreateMaterial(L"cloth", L"cartoon", L"OpaquePass", D3D11_CULL_FRONT, 2000);
	clothMat->SetTexture(m_mainLoader->m_textureLoader->GetByName(L"cloth_BaseColor")->shaderResourceView);
	clothMat->SetConstantBuffer(L"Test", &*m_testCB1, sizeof(TestConstantBuffer), 1);
	m_materialPool.push_back(clothMat);

	auto skyBoxMat = CreateMaterial(L"skyBox", L"skyBox", L"OpaquePass", D3D11_CULL_BACK, 2100);
	skyBoxMat->SetTexture(m_mainLoader->m_textureLoader->GetByName(L"skyBox_BaseColor")->shaderResourceView);
	skyBoxMat->SetConstantBuffer(L"Test", &*m_testCB2, sizeof(TestConstantBuffer), 1);
	m_materialPool.push_back(skyBoxMat);
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
		if ((*it)->objectName == L"cloth" || (*it)->objectName == L"face")
		{
			(*it)->SetRotation((*it)->transform->rotation.x, (*it)->transform->rotation.y + radians, (*it)->transform->rotation.z);
		}
	}
}
