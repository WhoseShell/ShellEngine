#pragma once
#include <ppltasks.h>
#include "BaseLoader.h"


namespace DX
{
#define MaxShaderNum 20

	struct Shader
	{
		std::wstring name;
		Microsoft::WRL::ComPtr<ID3D11VertexShader>	vertexShader;
		Microsoft::WRL::ComPtr<ID3D11PixelShader>	pixelShader;
		Microsoft::WRL::ComPtr<ID3D11InputLayout>	inputLayout;
		int IASize;
	};

	class ShaderLoader : public BaseLoader
	{
	public:
		ShaderLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		int LoadVertexShader(
			_In_ Platform::String^ filename, 
			_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[], 
			_In_ uint32 layoutDescNumElements
		);
		int LoadPiexelShader(_In_ Platform::String^ filename);
		bool LoadPSandVS(
			_In_ Platform::String^ VSfilename, 
			_In_ Platform::String^ PSfilename,
			_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC layoutDesc[], 
			_In_ uint32 layoutDescNumElements, 
			/*_In_opt_ std::vector<int>& count,*/
			_In_opt_ std::wstring name);

		std::vector<std::shared_ptr<Shader>>		shaderPool;

		std::shared_ptr<Shader> GetByName(std::wstring name);

	private:
		void CreateInputLayout(
			_In_reads_bytes_(bytecodeSize) byte* bytecode,
			_In_ uint32 bytecodeSize,
			_In_reads_opt_(layoutDescNumElements) D3D11_INPUT_ELEMENT_DESC* layoutDesc,
			_In_ uint32 layoutDescNumElements
		);

		std::vector<Microsoft::WRL::ComPtr<ID3D11VertexShader>>		allVertexShader;
		std::vector<Microsoft::WRL::ComPtr<ID3D11PixelShader>>		allPixelShader;
		std::vector<Microsoft::WRL::ComPtr<ID3D11InputLayout>>		allInputLayout;

		int vertexShaderLoadCount;
		int pixelShaderLoadCount;
	};
	
}