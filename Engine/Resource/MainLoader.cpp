#include "pch.h"
#include "MainLoader.h"
#include "..\Common\DirectXHelper.h"

using namespace DX;

MainLoader::MainLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice):m_d3dDevice(d3dDevice), constantBufferCount(0)
{
	m_shaderLoader = std::unique_ptr<ShaderLoader>( new ShaderLoader(d3dDevice));
	m_textureLoader = std::unique_ptr<TextureLoader>(new TextureLoader(d3dDevice));
	m_meshLoader = std::unique_ptr<MeshLoader>(new MeshLoader(d3dDevice));
}

int MainLoader::CreateConstantBuffer(unsigned int bufferSize)
{
	if (constantBufferCount == MaxCbufferNum)
	{
		return -1;
	}
	D3D11_BUFFER_DESC constantBufferDesc = { 0 };
	constantBufferDesc.ByteWidth = ((bufferSize + 15) / 16) * 16; // multiple of 16 bytes
	constantBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	constantBufferDesc.BindFlags = D3D11_BIND_CONSTANT_BUFFER;
	constantBufferDesc.CPUAccessFlags = 0;
	constantBufferDesc.MiscFlags = 0;
	// this will not be used as a structured buffer, so this parameter is ignored
	constantBufferDesc.StructureByteStride = 0;

	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&constantBufferDesc,
			nullptr,             // leave the buffer uninitialized
			&m_constantBuffer[constantBufferCount]
		)
	);

	return constantBufferCount++;
}

