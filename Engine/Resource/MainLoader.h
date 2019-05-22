#pragma once

#include "ShaderLoader.h"
#include "TextureLoader.h"
#include "MeshLoader.h"

namespace DX 
{
#define MaxCbufferNum 10

	class MainLoader 
	{

	public:
		MainLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		int CreateConstantBuffer(unsigned int bufferSize);
		
		ShaderLoader			* m_shaderLoader;
		TextureLoader			* m_textureLoader;
		MeshLoader				* m_meshLoader;

		Microsoft::WRL::ComPtr<ID3D11Buffer>                m_constantBuffer[MaxCbufferNum];

	private:
		Microsoft::WRL::ComPtr<ID3D11Device3> m_d3dDevice;
		int constantBufferCount;

	};
}