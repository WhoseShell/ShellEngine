#include "pch.h"
#include "EngineMain.h"
#include "Common\DirectXHelper.h"

using namespace Engine;
using namespace Windows::Foundation;
using namespace Windows::System::Threading;
using namespace Concurrency;
using namespace Windows::UI::Core;

// 加载应用程序时加载并初始化应用程序资产。
EngineMain::EngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources) :
	m_deviceResources(deviceResources)
{
	// 注册以在设备丢失或重新创建时收到通知
	m_deviceResources->RegisterDeviceNotify(this);

	// TODO: 将此替换为应用程序内容的初始化。
	//m_sceneRenderer = std::unique_ptr<Sample3DSceneRenderer>(new Sample3DSceneRenderer(m_deviceResources));
	//m_customSceneRenderer = std::unique_ptr<Sample3DScene>(new Sample3DScene(m_deviceResources)); //分配场景内存
	m_customSceneRenderer = std::unique_ptr<CartoonScene>(new CartoonScene(m_deviceResources)); //分配场景内存
	m_customSceneRenderer->Init(); //场景资源初始化
	

	m_fpsTextRenderer = std::unique_ptr<SampleFpsTextRenderer>(new SampleFpsTextRenderer(m_deviceResources));

	//添加交互事件监听
	m_userState = ref new UserState();
	CoreWindow::GetForCurrentThread()->KeyDown += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(m_userState, &UserState::OnKeyDown);
	CoreWindow::GetForCurrentThread()->KeyUp += ref new TypedEventHandler<CoreWindow^, KeyEventArgs^>(m_userState, &UserState::OnKeyUp); 
	//CoreWindow::GetForCurrentThread()->PointerMoved += ref new TypedEventHandler<CoreWindow^, PointerEventArgs^>(m_userState, &UserState::PointerMoved);
	Windows::Devices::Input::MouseDevice::GetForCurrentView()->MouseMoved +=
		ref new TypedEventHandler<Windows::Devices::Input::MouseDevice^, Windows::Devices::Input::MouseEventArgs^>(m_userState, &UserState::OnMouseMoved);


	m_customSceneRenderer->InputUserState(m_userState);

	// TODO: 如果需要默认的可变时间步长模式之外的其他模式，请更改计时器设置。
	// 例如，对于 60 FPS 固定时间步长更新逻辑，请调用:
	/*
	m_timer.SetFixedTimeStep(true);
	m_timer.SetTargetElapsedSeconds(1.0 / 60);
	*/

}

EngineMain::~EngineMain()
{
	// 取消注册设备通知
	m_deviceResources->RegisterDeviceNotify(nullptr);
}

// 在窗口大小更改(例如，设备方向更改)时更新应用程序状态
void EngineMain::CreateWindowSizeDependentResources() 
{
	// TODO: 将此替换为应用程序内容的与大小相关的初始化。
	//m_sceneRenderer->CreateWindowSizeDependentResources();
	
	m_customSceneRenderer->CreateWindowSizeDependentResources();
}

// 每帧更新一次应用程序状态。
void EngineMain::Update() 
{
	// 更新场景对象。
	m_timer.Tick([&]()
	{
		// TODO: 将此替换为应用程序内容的更新函数。
		//m_sceneRenderer->Update(m_timer);
		m_customSceneRenderer->Update(m_timer);
		m_fpsTextRenderer->Update(m_timer);
	});
}

// 根据当前应用程序状态呈现当前帧。
// 如果帧已呈现并且已准备好显示，则返回 true。
bool EngineMain::Render() 
{
	// 在首次更新前，请勿尝试呈现任何内容。
	if (m_timer.GetFrameCount() == 0)
	{
		return false;
	}

	auto context = m_deviceResources->GetD3DDeviceContext();

	// 呈现场景对象。
	// TODO: 将此替换为应用程序内容的渲染函数。
	//m_sceneRenderer->Render();
	m_customSceneRenderer->Render();
	m_fpsTextRenderer->Render();

	return true;
}

// 通知呈现器，需要释放设备资源。
void EngineMain::OnDeviceLost()
{
	//m_sceneRenderer->ReleaseDeviceDependentResources();
	m_customSceneRenderer->Release();
	m_fpsTextRenderer->ReleaseDeviceDependentResources();
}

// 通知呈现器，现在可重新创建设备资源。
void EngineMain::OnDeviceRestored()
{
	//m_sceneRenderer->CreateDeviceDependentResources();
	m_customSceneRenderer->Init();
	m_fpsTextRenderer->CreateDeviceDependentResources();
	CreateWindowSizeDependentResources();
}
