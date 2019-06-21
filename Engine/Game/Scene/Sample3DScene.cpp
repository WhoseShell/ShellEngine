#include "pch.h"
#include "Sample3DScene.h"
#include "..\Common\DirectXHelper.h"


using namespace std;
using namespace Windows::Foundation;
using namespace Engine;
using namespace DirectX;

Engine::Sample3DScene::Sample3DScene(const std::shared_ptr<DX::DeviceResources>& deviceResources)
	:BaseScene(deviceResources),
	m_degreesPerSecond(45)
{
	m_sampleRenderer = std::shared_ptr<SampleRenderer>(new SampleRenderer(deviceResources, m_mainLoader, m_renderData, m_constantData)); //����Renderer
	m_constantBufferData = std::shared_ptr<MVPConstantBuffer>(new MVPConstantBuffer);
	m_moveController = std::unique_ptr<MoveController>(new MoveController());
}

void Engine::Sample3DScene::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// ����һ���򵥵ĸ���ʾ������Ӧ�ó�����������ͼ�������ͼ��ʱ�����Խ��д˸���
	//��
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// ��ע�⣬OrientationTransform3D �����ڴ˴��Ǻ�˵ģ�
	// ����ȷȷ�������ķ���ʹ֮����ʾ����ƥ�䡣
	// ���ڽ�������Ŀ��λͼ���е��κλ��Ƶ���
	// ����������Ŀ�ꡣ���ڵ�����Ŀ��Ļ��Ƶ��ã�
	// ��ӦӦ�ô�ת����

	// ��ʾ��ʹ�����������������������ϵ��
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

	// �۾�λ��(0,0.7,1.5)�������� Y ��ʹ������ʸ�����ҵ�(0,-0.1,0)��
	eye = { 0.0f, 0.0f, -3.0f};
	at = { 0.0f, 0.0f, 1.0f};
	up = { 0.0f, 1.0f, 0.0f};

	DirectX::XMStoreFloat4x4(&(m_constantBufferData->view), XMMatrixLookAtRH(DirectX::XMLoadFloat3(&eye), DirectX::XMLoadFloat3(&at), DirectX::XMLoadFloat3(&up)));
}

void Engine::Sample3DScene::Init()
{
#pragma region ����VB/VS����ѡ��
	Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer1;
	uint32 vertexStride1 = sizeof(VertexPosColor);

	static const VertexPosColor cubeVertices[] =
	{
		{XMFLOAT3(-0.5f, -0.5f, -0.5f), XMFLOAT3(0.0f, 0.0f, 0.0f), XMFLOAT2(0.0f, 0.0f)},
		{XMFLOAT3(-0.5f, -0.5f,  0.5f), XMFLOAT3(0.0f, 0.0f, 1.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(-0.5f,  0.5f, -0.5f), XMFLOAT3(0.0f, 1.0f, 0.0f), XMFLOAT2(0.0f, 1.0f)},
		{XMFLOAT3(-0.5f,  0.5f,  0.5f), XMFLOAT3(0.0f, 1.0f, 1.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(0.5f, -0.5f, -0.5f),  XMFLOAT3(1.0f, 0.0f, 0.0f), XMFLOAT2(1.0f, 0.0f)},
		{XMFLOAT3(0.5f, -0.5f,  0.5f), XMFLOAT3(1.0f, 0.0f, 1.0f), XMFLOAT2(2.0f, 0.0f)},
		{XMFLOAT3(0.5f,  0.5f, -0.5f), XMFLOAT3(1.0f, 1.0f, 0.0f), XMFLOAT2(1.0f, 1.0f)},
		{XMFLOAT3(0.5f,  0.5f,  0.5f), XMFLOAT3(1.0f, 1.0f, 1.0f), XMFLOAT2(2.0f, 1.0f)},
	};
	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = cubeVertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(sizeof(cubeVertices), D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			vertexBuffer1.GetAddressOf()
		)
	);


	// ��������������ÿ����������ʾ
// Ҫ����Ļ�ϳ��ֵ������Ρ�
// ����: 0,2,1 ��ʾ���������
// ���㻺�����е�����Ϊ 0��2 �� 1 �Ķ��㹹����
// ������ĵ�һ�������Ρ�
	static const unsigned short cubeIndices[] =
	{
		0,2,1, // -x
		1,2,3,

		4,5,6, // +x
		5,7,6,

		0,1,5, // -y
		0,5,4,

		6,7,3, // +y
		3,2,6,

		0,4,2, // -z
		4,6,2,

		5,1,3, // +z
		3,7,5,
	};

	Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer1;
	int indexCount1 = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			indexBuffer1.GetAddressOf()
		)
	);
#pragma endregion

#pragma region ����VB/VS

	Microsoft::WRL::ComPtr<ID3D11Buffer>		marble2Buffer1;
	Microsoft::WRL::ComPtr<ID3D11Buffer>		marble2IndexBuffer1;
	uint32 marble2IndexCount;
	uint32 marble2VertexCount;
	uint32 marble2VertexStride;
	
	m_mainLoader->m_meshLoader->LoadMesh(L"Assets\\Sphere.bin", marble2Buffer1.GetAddressOf(), marble2IndexBuffer1.GetAddressOf(), &marble2VertexCount, &marble2IndexCount, &marble2VertexStride);

#pragma endregion

#pragma region ����VS/PS/IA
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	D3D11_INPUT_ELEMENT_DESC marble2Desc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 40, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		//{ "TANGENT", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 32, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};

	std::vector<int> shaderConut;
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", vertexDesc, ARRAYSIZE(vertexDesc), shaderConut);
	//m_mainLoader->m_shaderLoader->LoadPSandVS(L"BasicVertexShader.cso", L"BasicPixelShader.cso", marble2Desc, ARRAYSIZE(marble2Desc), shaderConut);
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", marble2Desc, ARRAYSIZE(marble2Desc), shaderConut);
	
#pragma endregion

#pragma region ����ConstantBuffer / ����װ��ConstantData
	Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer1;
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer) + 12, D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			constantBuffer1.GetAddressOf()
		)
	);
	CreateWindowSizeDependentResources(); // ����m_constantBufferData;
	m_constantData->mvp = m_constantBufferData;
#pragma endregion

#pragma region ������ͼ��SRV
	int SRVID1 = m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\floor_section1.dds", L"diffuse");
	int SRVID2 = m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\LockScreenLogo.scale-200.png", L"diffuse");
#pragma endregion

#pragma region ����������
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

#pragma region װ��PerObject
	for (size_t i = 0; i < 4; i++)
	{
		auto currentObj = std::shared_ptr<PerObjectData>(new PerObjectData);//����һ����Ⱦ����

		//���øö���name��shaderNama
		currentObj->objectName = wstring(L"����") + (i.ToString())->Data();
		currentObj->shaderName = wstring(L"Sample3DPass");
		currentObj->renderQueue = 2000;

		//Buffer
		currentObj->vertexBuffer = vertexBuffer1;
		currentObj->indexBuffer = indexBuffer1;
		currentObj->indexCount = indexCount1;
		currentObj->constantBuffer = constantBuffer1;
		currentObj->vertexStride = vertexStride1;

		// IA/VS/PS
		currentObj->vertexShader = m_mainLoader->m_shaderLoader->allVertexShader[0];
		currentObj->pixelShader = m_mainLoader->m_shaderLoader->allPixelShader[0];
		currentObj->inputLayout = m_mainLoader->m_shaderLoader->allInputLayout[0];
		
		//SRV��Texture/Transform
		if (i == 0 || i == 1)//���ص�marble Mesh
		{
			currentObj->objectName = L"marble";
			currentObj->vertexBuffer = marble2Buffer1;
			currentObj->indexBuffer = marble2IndexBuffer1;
			currentObj->indexCount = marble2IndexCount;
			currentObj->vertexStride = marble2VertexStride;
			currentObj->vertexShader = m_mainLoader->m_shaderLoader->allVertexShader[1];
			currentObj->pixelShader = m_mainLoader->m_shaderLoader->allPixelShader[1];
			currentObj->inputLayout = m_mainLoader->m_shaderLoader->allInputLayout[1];
			currentObj->baseColor = m_mainLoader->m_textureLoader->allSRV[SRVID1].SRV;
			XMStoreFloat4x4(&currentObj->transform, XMMatrixTranslation(0.0f, 0.0f, 0.0f));
			if (i == 1)
			{
				currentObj->baseColor = m_mainLoader->m_textureLoader->allSRV[SRVID2].SRV;
				XMStoreFloat4x4(&currentObj->transform, XMMatrixTranslation(0.0f, 2.0f, 0.0f));
			}
		}
		if (i == 2)
		{
			currentObj->baseColor = m_mainLoader->m_textureLoader->allSRV[SRVID2].SRV;
			XMStoreFloat4x4(&currentObj->transform, XMMatrixTranslation(2.0f, 0.0f, 0.0f));
		}
		if (i == 3)
		{
			currentObj->baseColor = m_mainLoader->m_textureLoader->allSRV[SRVID2].SRV;
			XMStoreFloat4x4(&currentObj->transform, XMMatrixTranslation(-2.0f, 0.0f, 0.0f));
		}
		m_renderData->perObject.push_back(currentObj);//�������������
	}
#pragma endregion

}

void Engine::Sample3DScene::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		//Rotate(0.01);
		//Rotate(0);
		m_constantBufferData->time = (float)timer.GetTotalSeconds();
	}
	if (m_moveController != nullptr)
	{
		m_moveController->MoveCamera(m_constantBufferData->view, u_state, eye, at, up);
	}
}

void Engine::Sample3DScene::Render()
{
	auto context = m_deviceResources->GetD3DDeviceContext();

	m_sampleRenderer->SetPass(); //����renderPass

	m_sampleRenderer->ExecuteSequentially();
}

void Engine::Sample3DScene::Release()
{
	m_sampleRenderer->FrameReleaseSequentially();
	m_renderData->perObject.clear();
}

void Engine::Sample3DScene::Rotate(float radians)
{
	// ׼�������µ�ģ�;��󴫵ݵ���ɫ��
	for (vector<shared_ptr<PerObjectData>>::iterator it = m_renderData->perObject.begin();it != m_renderData->perObject.end();it++)
	{
		XMStoreFloat4x4(&(*it)->transform, XMMatrixMultiply(XMMatrixRotationY(radians), XMLoadFloat4x4(&(*it)->transform)));
	}

	//XMStoreFloat4x4(&m_constantBufferData->model, XMMatrixRotationY(radians));
}
