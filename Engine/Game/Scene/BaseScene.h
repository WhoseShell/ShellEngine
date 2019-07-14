#pragma once
#include "ResourceLoad\MainLoader.h"
#include "RenderPipeline\Pass\BasePass.h"
#include "Game\State\UserState.h"
#include"Common\Type\Material.h"
#include"Common\Type\Object.h"

namespace Engine
{
	class BaseScene
	{
	public:
		BaseScene(
			const std::shared_ptr<DX::DeviceResources>& deviceResources
		);

		void InputUserState(UserState^ ustate);

	protected:
		///////////////////////////////////////////////////////////////////////////
		// RenderResource

		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;

		///////////////////////////////////////////////////////////////////////////
		// ConstantBuffer

	protected:

		XMFLOAT3 eye;
		XMFLOAT3 at;
		XMFLOAT3 up;

		Microsoft::WRL::ComPtr<ID3D11Buffer> m_constantBuffer;
		std::shared_ptr<GlobalConstantBuffer> m_globalConstantData;
		std::shared_ptr<MVPConstantBuffer> m_MVPConstantData;
		std::shared_ptr<ScatterConstantBuffer> m_scatterConstantData;

		void ResetScatterProperty();

	private:

		void CreateGlobalConstantBuffer();

		/////////////////////////////////////////////////////////////////////////////
		// Mat & Object

	protected:
		std::vector<std::shared_ptr<Material>> m_materialPool;
		std::shared_ptr<Material> CreateMaterial(
			std::wstring matName,
			std::wstring shaderName,
			std::wstring passName,
			D3D11_CULL_MODE cullMode,
			int renderQueue
		);

		std::shared_ptr<Material> GetMatByName(std::wstring matName);
		std::shared_ptr<Object> GetObjectByNmae(std::wstring objName);
		void AssembObject(
			std::shared_ptr<Object> &object,
			std::wstring objectName, 
			std::wstring meshName, 
			std::wstring matName,
			std::shared_ptr<Transform> transform,
			int indexCount);

		/////////////////////////////////////////////////////////////////////////////
		// state

	protected:
		UserState^  m_userState;

		/////////////////////////////////////////////////////////////////////////////
	};
}