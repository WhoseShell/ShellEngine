﻿#pragma once

#include "Common\StepTimer.h"
#include "Common\DeviceResources.h"
#include "Content\Sample3DSceneRenderer.h"
#include "Content\SampleFpsTextRenderer.h"
#include "Game\Scene\Sample3DScene.h"

// 在屏幕上呈现 Direct2D 和 3D 内容。
namespace Engine
{
	class EngineMain : public DX::IDeviceNotify
	{
	public:
		EngineMain(const std::shared_ptr<DX::DeviceResources>& deviceResources);
		~EngineMain();
		void CreateWindowSizeDependentResources();
		void Update();
		bool Render();

		// IDeviceNotify
		virtual void OnDeviceLost();
		virtual void OnDeviceRestored();

	private:
		// 缓存的设备资源指针。
		std::shared_ptr<DX::DeviceResources> m_deviceResources;

		// TODO: 替换为你自己的内容呈现器。
		std::unique_ptr<Sample3DSceneRenderer> m_sceneRenderer;
		std::unique_ptr<Sample3DScene> m_customSceneRenderer;
		std::unique_ptr<SampleFpsTextRenderer> m_fpsTextRenderer;

		// 渲染循环计时器。
		DX::StepTimer m_timer;
	};
}