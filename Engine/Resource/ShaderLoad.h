#pragma once
#include <ppltasks.h>
#include "..\Common\DeviceResources.h"

namespace DX
{
#define MaxShaderNum 20

	class ShaderLoad
	{
	public:
		std::shared_ptr<DX::DeviceResources> m_deviceResources;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	allVertexShader[MaxShaderNum];
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	allPixelShader[MaxShaderNum];
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	allInputLayout[MaxShaderNum];

	private:
		int vertexShaderLoadCount;
		int pixelShaderLoadCount;

	public:
		ShaderLoad(const std::shared_ptr<DX::DeviceResources>& deviceResources);

		void LoadVertexShader(const std::wstring& filename);

		void LoadPiexelShader(const std::wstring& filename);

	};
	
}