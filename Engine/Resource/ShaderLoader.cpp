#include "pch.h"
#include "ShaderLoader.h"
#include "..\Common\DirectXHelper.h"

using namespace DX;

ShaderLoader::ShaderLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice) : BaseLoader(d3dDevice), vertexShaderLoadCount(0), pixelShaderLoadCount(0)
{

};
//https://docs.microsoft.com/zh-cn/windows/uwp/gaming/creating-shaders-and-drawing-primitives
int ShaderLoader::LoadVertexShader(_In_ Platform::String^ filename, _In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[], _In_ uint32 layoutDescNumElements)
{
	Microsoft::WRL::ComPtr<ID3D11VertexShader> vs;
	allVertexShader.push_back(vs);
	Platform::Array<byte>^ bytecode = ReadData(filename);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateVertexShader(
			bytecode->Data,
			bytecode->Length,
			nullptr,
			&(allVertexShader[vertexShaderLoadCount])
		)
	);

	CreateInputLayout(
		bytecode->Data,
		bytecode->Length,
		layoutDesc,
		layoutDescNumElements
	);


	return vertexShaderLoadCount++;
}

int ShaderLoader::LoadPiexelShader(_In_ Platform::String^ filename)
{
	Microsoft::WRL::ComPtr<ID3D11PixelShader> ps;
	allPixelShader.push_back(ps);

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

bool DX::ShaderLoader::LoadPSandVS(
	_In_ Platform::String^ VSfilename, 
	_In_ Platform::String^ PSfilename, 
	_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[], 
	_In_ uint32 layoutDescNumElements, 
	std::vector<int>& count)
{
	if (vertexShaderLoadCount == MaxShaderNum || pixelShaderLoadCount == MaxShaderNum)
	{
		return false;
	}
	count.push_back(ShaderLoader::LoadVertexShader(VSfilename,  layoutDesc, layoutDescNumElements));
	count.push_back(ShaderLoader::LoadPiexelShader(PSfilename));
	count.push_back(count.at(1));
	return true;
}

void ShaderLoader::CreateInputLayout(
	byte * bytecode, 
	uint32 bytecodeSize, 
	D3D11_INPUT_ELEMENT_DESC * layoutDesc, 
	uint32 layoutDescNumElements)
{
	Microsoft::WRL::ComPtr<ID3D11InputLayout> ia;
	allInputLayout.push_back(ia);

	if (layoutDesc == nullptr)
	{
		// If no input layout is specified, use the BasicVertex layout.
		const D3D11_INPUT_ELEMENT_DESC basicVertexLayoutDesc[] =
		{
			{ "POSITION", 0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 0,  D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "NORMAL",   0, DXGI_FORMAT_R32G32B32_FLOAT, 0, 12, D3D11_INPUT_PER_VERTEX_DATA, 0 },
			{ "TEXCOORD", 0, DXGI_FORMAT_R32G32_FLOAT,    0, 24, D3D11_INPUT_PER_VERTEX_DATA, 0 },
		};

		DX::ThrowIfFailed(
			m_d3dDevice->CreateInputLayout(
				basicVertexLayoutDesc,
				ARRAYSIZE(basicVertexLayoutDesc),
				bytecode,
				bytecodeSize,
				&allInputLayout[vertexShaderLoadCount]
			)
		);
	}
	else
	{
		DX::ThrowIfFailed(
			m_d3dDevice->CreateInputLayout(
				layoutDesc,
				layoutDescNumElements,
				bytecode,
				bytecodeSize,
				&allInputLayout[vertexShaderLoadCount]
			)
		);
	}
}



