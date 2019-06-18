#pragma once
#include <ppltasks.h>
#include "BasicLoader.h"
#include "BaseLoader.h"

namespace DX
{
	struct SRVStruct
	{
		int id;
		std::wstring name;
		Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> SRV;
		//ID3D11ShaderResourceView* SRV;
	};
	class TextureLoader : public BaseLoader
	{
	public:
		std::vector<SRVStruct> allSRV;
		BasicLoader^ loader;
		TextureLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		int LoadToSRV(_In_ wchar_t const* filename, std::wstring matName);
		int LoadToSRV(wchar_t const* filename, std::wstring matName, _Out_opt_ ID3D11ShaderResourceView** textureView);
	protected:

	private:
		/*Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wicFactory;*/
		int SRVLoadCount;
	};

}