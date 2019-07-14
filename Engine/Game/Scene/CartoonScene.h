#pragma once
#include "..\ResourceLoad\MainLoader.h"
#include "..\Common\StepTimer.h"
#include "BaseScene.h"
#include "RenderPipeline\Renderer\CartoonRenderer.h"
#include "RenderPipeline\Pass\ShaderStruct.h"
#include "Game\Controller\MoveController.h"

using namespace DX;

namespace Engine
{
	class CartoonScene : public BaseScene
	{
	public:
		CartoonScene(
			const std::shared_ptr<DX::DeviceResources>& deviceResources
		);
		void CreateWindowSizeDependentResources();
		void Update(DX::StepTimer const& timer);
		void Render();
		void LoadResource();
		void Init();
		void Release();

	private:
		// 用于渲染循环的变量。
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;

		std::shared_ptr<TestConstantBuffer> m_testCB0;
		std::shared_ptr<TestConstantBuffer> m_testCB1;
		std::shared_ptr<TestConstantBuffer> m_testCB2;


		std::unique_ptr<MoveController> m_moveController;

		std::shared_ptr<CartoonRenderer> m_cartoonRenderer;
		void Rotate(float radians);

	};
}