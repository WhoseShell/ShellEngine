#pragma once
#include <ppltasks.h>
#include "BaseLoader.h"

namespace DX
{
	struct SRVStruct
	{
		int id;
		std::wstring name;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;
	};
	class TextureLoader : public BaseLoader
	{
	public:
		TextureLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		int LoadToSRV(_In_ Platform::String^ filename, std::wstring matName);
	protected:
		void CreateTexture(
			_In_ bool decodeAsDDS,
			_In_reads_bytes_(dataSize) byte* data,
			_In_ uint32 dataSize,
			_Out_opt_ ID3D11Texture2D** texture,
			_Out_opt_ ID3D11ShaderResourceView** textureView,
			_In_opt_ Platform::String^ debugName
		);

		void LoadTexture(
			_In_ Platform::String^ filename,
			_Out_opt_ ID3D11Texture2D** texture,
			_Out_opt_ ID3D11ShaderResourceView** textureView
		);

		Platform::String^ GetExtension(
			_In_ Platform::String^ filename
		);

		std::vector<SRVStruct> allSRV;


	private:
		Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wicFactory;
		int SRVLoadCount;
	};

}