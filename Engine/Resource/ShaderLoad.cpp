#include "pch.h"
#include "ShaderLoad.h"
#include "..\Common\DirectXHelper.h"


namespace DX
{
	ShaderLoad::ShaderLoad(const std::shared_ptr<DX::DeviceResources>& deviceResources) :m_deviceResources(deviceResources), vertexShaderLoadCount(0), pixelShaderLoadCount(0){};
	//https://docs.microsoft.com/zh-cn/windows/uwp/gaming/creating-shaders-and-drawing-primitives
	void ShaderLoad::LoadVertexShader(const std::wstring& filename)
	{
		if (vertexShaderLoadCount == MaxShaderNum) //shader达到加载上限则返回
		{
			return;
		}

		auto loadVSTask = DX::ReadDataAsync(filename);

		auto createVSTask = loadVSTask.then([this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateVertexShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&allVertexShader[vertexShaderLoadCount]
				)
			);

			static const D3D11_INPUT_ELEMENT_DESC vertexDesc[] =
			{
				{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0, D3D11_INPUT_PER_VERTEX_DATA, 0 },
				{ "COLOR", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			};

			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreateInputLayout(
					vertexDesc,
					ARRAYSIZE(vertexDesc),
					&fileData[0],
					fileData.size(),
					&allInputLayout[vertexShaderLoadCount]
				)
			);
		});

		vertexShaderLoadCount++;
	}

	void ShaderLoad::LoadPiexelShader(const std::wstring & filename)
	{
		if (pixelShaderLoadCount == MaxShaderNum) //shader达到加载上限则返回
		{
			return;
		}
		auto loadPSTask = DX::ReadDataAsync(filename);
		auto createPSTask = loadPSTask.then([this](const std::vector<byte>& fileData) {
			DX::ThrowIfFailed(
				m_deviceResources->GetD3DDevice()->CreatePixelShader(
					&fileData[0],
					fileData.size(),
					nullptr,
					&allPixelShader[pixelShaderLoadCount]
				)
			);
		});

		pixelShaderLoadCount++;
	}

}
