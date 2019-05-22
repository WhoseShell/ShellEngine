#pragma once
#include <ppltasks.h>
#include "BaseLoader.h"


namespace DX
{
#define MaxShaderNum 20

	class ShaderLoader : public BaseLoader
	{
	public:
		ShaderLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		int LoadVertexShader(_In_ Platform::String^ filename);
		int LoadPiexelShader(_In_ Platform::String^ filename);
		bool LoadPSandVS(_In_ Platform::String^ VSfilename, _In_ Platform::String^ PSfilename, std::vector<int>& count);

		Microsoft::WRL::ComPtr<ID3D11VertexShader>	allVertexShader[MaxShaderNum];
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	allPixelShader[MaxShaderNum];
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	allInputLayout[MaxShaderNum];

	private:
		int vertexShaderLoadCount;
		int pixelShaderLoadCount;
	};
	
}