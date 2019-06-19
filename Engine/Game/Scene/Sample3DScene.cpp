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
	Init(); //������Դ��ʼ��
	m_sampleRenderer = std::shared_ptr<SampleRenderer>(new SampleRenderer(deviceResources, m_mainLoader, m_renderData)); //����Renderer
	m_sampleRenderer->SetPass(); //����renderPass
	CreateWindowSizeDependentResources();
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

	XMMATRIX orientationMatrix = XMLoadFloat4x4(&orientation);

	XMStoreFloat4x4(
		&m_constantBufferData.projection,
		perspectiveMatrix * orientationMatrix
	);

	// �۾�λ��(0,0.7,1.5)�������� Y ��ʹ������ʸ�����ҵ�(0,-0.1,0)��
	eye = { 0.0f, 0.0f, -3.0f};
	at = { 0.0f, 0.0f, 1.0f};
	up = { 0.0f, 1.0f, 0.0f};

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixLookAtRH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up)));
}

void Engine::Sample3DScene::Init()
{
	m_renderData->perObject.push_back(std::shared_ptr<PerObjectData>(new PerObjectData));//����һ����Ⱦ����

	//���øö���name��shaderNama
	m_renderData->perObject.at(0)->objectName = wstring(L"����");
	m_renderData->perObject.at(0)->shaderName = wstring(L"Sample3DPass");

	//����VS/PS/IA, ���õ�renderData
	D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
	{
		{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		{ "UV", 0, DXGI_FORMAT_R32G32_FLOAT, 0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
	};
	std::vector<int> shaderConut;
	m_mainLoader->m_shaderLoader->LoadPSandVS(L"SampleVertexShader.cso", L"SamplePixelShader.cso", vertexDesc, ARRAYSIZE(vertexDesc), shaderConut);
	m_renderData->perObject.at(0)->vertexShader = m_mainLoader->m_shaderLoader->allVertexShader[shaderConut[0]];
	m_renderData->perObject.at(0)->pixelShader = m_mainLoader->m_shaderLoader->allPixelShader[shaderConut[1]];
	m_renderData->perObject.at(0)->inputLayout = m_mainLoader->m_shaderLoader->allInputLayout[shaderConut[2]];

	//����ConstantBuffer�����õ�renderData
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer)+12, D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&(m_renderData->perObject.at(0)->constantBuffer)
		)
	);

	//����mesh
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
			&(m_renderData->perObject.at(0)->vertexBuffer)
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

	m_renderData->perObject.at(0)->indexCount = ARRAYSIZE(cubeIndices);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = cubeIndices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(sizeof(cubeIndices), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			&(m_renderData->perObject.at(0)->indexBuffer)
		)
	);
	
	// Reset the viewport to target the whole screen.
	auto viewport = m_deviceResources->GetScreenViewport();
	m_deviceResources->GetD3DDeviceContext()->RSSetViewports(1, &viewport);

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

	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\floor_section1.dds", L"diffuse");
	m_renderData->perObject.at(0)->baseColor = m_mainLoader->m_textureLoader->allSRV[0].SRV;
}

void Engine::Sample3DScene::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// ����ת���ɻ��ȣ�Ȼ����ת��Ϊ��ת�Ƕ�
		float radiansPerSecond = XMConvertToRadians(m_degreesPerSecond);
		double totalRotation = timer.GetTotalSeconds() * radiansPerSecond;
		float radians = static_cast<float>(fmod(totalRotation, XM_2PI));

		Rotate(radians);
		//Rotate(0);
		m_constantBufferData.time = (float)timer.GetTotalSeconds();
	}
	if (m_moveController != nullptr)
	{
		m_moveController->MoveCamera(m_constantBufferData.view, u_state, eye, at, up);
	}
}

void Engine::Sample3DScene::Render()
{
	auto context = m_deviceResources->GetD3DDeviceContext();


	if (!m_renderData->perObject.empty()) 
	{
		context->UpdateSubresource1(
			m_renderData->perObject.at(0)->constantBuffer.Get(),
			0,
			NULL,
			&m_constantBufferData,
			0,
			0,
			0
		);
	}

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
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixRotationY(radians));
}
