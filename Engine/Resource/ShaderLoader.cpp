#include "pch.h"
#include "ShaderLoader.h"
#include "..\Common\DirectXHelper.h"

using namespace DX;

ShaderLoader::ShaderLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice) : BaseLoader(d3dDevice), vertexShaderLoadCount(0), pixelShaderLoadCount(0) {	};
//https://docs.microsoft.com/zh-cn/windows/uwp/gaming/creating-shaders-and-drawing-primitives
int ShaderLoader::LoadVertexShader(_In_ Platform::String^ filename)
{
	if (vertexShaderLoadCount == MaxShaderNum) //shader达到加载上限则返回
	{
		return -1;
	}
	Platform::Array<byte>^ bytecode = ReadData(filename);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateVertexShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			&allVertexShader[0]
		)
	);
	return vertexShaderLoadCount++;
}

int ShaderLoader::LoadPiexelShader(_In_ Platform::String^ filename)
{
	if (pixelShaderLoadCount == MaxShaderNum) //shader达到加载上限则返回
	{
		return -1;
	}
	Platform::Array<byte>^ bytecode = ReadData(filename);
	
		DX::ThrowIfFailed(
			m_d3dDevice->CreatePixelShader(
				bytecode->Data,
				bytecode->Length,
				nullptr,
				&allPixelShader[pixelShaderLoadCount]
			)
		);

	return pixelShaderLoadCount++;
}

bool DX::ShaderLoader::LoadPSandVS(Platform::String ^ VSfilename, Platform::String ^ PSfilename, std::vector<int>& count)
{
	if (vertexShaderLoadCount == MaxShaderNum || pixelShaderLoadCount == MaxShaderNum)
	{
		return false;
	}
	count[0] = ShaderLoader::LoadVertexShader(VSfilename);
	count[1] = ShaderLoader::LoadPiexelShader(PSfilename);

	return true;
}



