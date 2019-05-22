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
		int LoadVertexShader(_In_ Platform::String^ filename, _In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[], _In_ uint32 layoutDescNumElements, _Out_opt_ Microsoft::WRL::ComPtr<ID3D11InputLayout> layout);
		int LoadPiexelShader(_In_ Platform::String^ filename);
		bool LoadPSandVS(
			_In_ Platform::String^ VSfilename, 
			_In_ Platform::String^ PSfilename,
			_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[], 
			_In_ uint32 layoutDescNumElements, 
			_Out_opt_ Microsoft::WRL::ComPtr<ID3D11InputLayout> layout, 
			std::vector<int>& count);

		Microsoft::WRL::ComPtr<ID3D11VertexShader>	allVertexShader[MaxShaderNum];
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	allPixelShader[MaxShaderNum];
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	allInputLayout[MaxShaderNum];

	private:
		void CreateInputLayout(
			_In_reads_bytes_(bytecodeSize) byte* bytecode,
			_In_ uint32 bytecodeSize,
			_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC* layoutDesc,
			_In_ uint32 layoutDescNumElements
		);

		int vertexShaderLoadCount;
		int pixelShaderLoadCount;
	};
	
}