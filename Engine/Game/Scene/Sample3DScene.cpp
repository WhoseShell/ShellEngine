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
	Init(); //场景资源初始化
	m_sampleRenderer = std::shared_ptr<SampleRenderer>(new SampleRenderer(deviceResources, m_mainLoader, m_renderData)); //创建Renderer
	m_sampleRenderer->SetPass(); //设置renderPass
	CreateWindowSizeDependentResources();
	m_moveController = std::unique_ptr<MoveController>(new MoveController());
	
}

void Engine::Sample3DScene::CreateWindowSizeDependentResources()
{
	Size outputSize = m_deviceResources->GetOutputSize();
	float aspectRatio = outputSize.Width / outputSize.Height;
	float fovAngleY = 70.0f * XM_PI / 180.0f;

	// 这是一个简单的更改示例，当应用程序在纵向视图或对齐视图中时，可以进行此更改
	//。
	if (aspectRatio < 1.0f)
	{
		fovAngleY *= 2.0f;
	}

	// 请注意，OrientationTransform3D 矩阵在此处是后乘的，
	// 以正确确定场景的方向，使之与显示方向匹配。
	// 对于交换链的目标位图进行的任何绘制调用
	// 交换链呈现目标。对于到其他目标的绘制调用，
	// 不应应用此转换。

	// 此示例使用行主序矩阵利用右手坐标系。
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

	// 眼睛位于(0,0.7,1.5)，并沿着 Y 轴使用向上矢量查找点(0,-0.1,0)。
	eye = { 0.0f, 0.0f, -3.0f};
	at = { 0.0f, 0.0f, 1.0f};
	up = { 0.0f, 1.0f, 0.0f};

	XMStoreFloat4x4(&m_constantBufferData.view, XMMatrixLookAtRH(XMLoadFloat3(&eye), XMLoadFloat3(&at), XMLoadFloat3(&up)));
}

void Engine::Sample3DScene::Init()
{
	m_renderData->perObject.push_back(std::shared_ptr<PerObjectData>(new PerObjectData));//创建一个渲染对象

	//设置该对象name及shaderNama
	m_renderData->perObject.at(0)->objectName = wstring(L"方块");
	m_renderData->perObject.at(0)->shaderName = wstring(L"Sample3DPass");

	//创建VS/PS/IA, 设置到renderData
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

	//创建ConstantBuffer，设置到renderData
	CD3D11_BUFFER_DESC constantBufferDesc(sizeof(MVPConstantBuffer)+12, D3D11_BIND_CONSTANT_BUFFER);
	DX::ThrowIfFailed(
		m_deviceResources->GetD3DDevice()->CreateBuffer(
			&constantBufferDesc,
			nullptr,
			&(m_renderData->perObject.at(0)->constantBuffer)
		)
	);

	//创建mesh
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

	// 加载网格索引。每三个索引表示
	// 要在屏幕上呈现的三角形。
	// 例如: 0,2,1 表示顶点的索引
	// 顶点缓冲区中的索引为 0、2 和 1 的顶点构成了
	// 此网格的第一个三角形。
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

	m_mainLoader->m_textureLoader->LoadToSRV(L"Assets\\floor_section1.dds", L"diffuse");
	m_renderData->perObject.at(0)->baseColor = m_mainLoader->m_textureLoader->allSRV[0].SRV;
}

void Engine::Sample3DScene::Update(DX::StepTimer const& timer)
{
	if (!m_tracking)
	{
		// 将度转换成弧度，然后将秒转换为旋转角度
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

	m_sampleRenderer->SetPass(); //设置renderPass
	m_sampleRenderer->ExecuteSequentially();
}

void Engine::Sample3DScene::Release()
{
	m_sampleRenderer->FrameReleaseSequentially();
	m_renderData->perObject.clear();
}

void Engine::Sample3DScene::Rotate(float radians)
{
	// 准备将更新的模型矩阵传递到着色器
	XMStoreFloat4x4(&m_constantBufferData.model, XMMatrixRotationY(radians));
}
