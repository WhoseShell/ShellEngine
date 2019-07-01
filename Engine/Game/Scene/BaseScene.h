#pragma once
#include "..\Resource\MainLoader.h"
#include "Pass\BasePass.h"
#include "Game\UserState.h"
#include"Game\Material.h"
#include"Game\Object.h"

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
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		std::shared_ptr<DX::MainLoader> m_mainLoader;
		std::shared_ptr<RenderData> m_renderData;
		std::shared_ptr<ConstantData> m_constantData;
		UserState^  u_state;

		//use in ResourceInit
		//Microsoft::WRL::ComPtr<ID3D11Buffer>		vertexBuffer;
		//Microsoft::WRL::ComPtr<ID3D11Buffer>		indexBuffer;
		//uint32 indexCount;
		//uint32 vertexCount;
		//uint32 vertexStride;
		Microsoft::WRL::ComPtr<ID3D11Buffer> constantBuffer;

		std::vector<std::shared_ptr<Material>> materialPool;
		std::shared_ptr<Material> CreateMaterial(
			std::wstring shaderName,
			std::wstring matName,
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
			XMMATRIX &transform,
			int indexCount);
	};
}