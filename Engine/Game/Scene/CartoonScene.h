#pragma once
#include "..\Resource\MainLoader.h"
#include "..\Common\StepTimer.h"
#include "BaseScene.h"
#include "Renderer\CartoonRenderer.h"
#include "Pass\ShaderStruct.h"
#include "Game\MoveController.h"

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
		void Init();
		void Release();

	private:
		// 用于渲染循环的变量。
		bool	m_loadingComplete;
		float	m_degreesPerSecond;
		bool	m_tracking;

		XMFLOAT3 eye;
		XMFLOAT3 at;
		XMFLOAT3 up;

		std::shared_ptr<MVPConstantBuffer> m_constantBufferData;
		std::unique_ptr<MoveController> m_moveController;

		std::shared_ptr<CartoonRenderer> m_cartoonRenderer;
		void Rotate(float radians);

	};
}