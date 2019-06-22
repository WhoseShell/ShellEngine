#pragma once
#include <ppltasks.h>
#include "BasicLoader.h"
#include "BaseLoader.h"
using namespace Microsoft::WRL;
namespace DX
{
	struct SRV
	{
		std::wstring name;
		ComPtr<ID3D11ShaderResourceView> shaderResourceView;
	};
	class TextureLoader : public BaseLoader
	{
	public:
		std::vector<std::shared_ptr<SRV>> SRVPool;
		BasicLoader^ loader;
		TextureLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice);
		int LoadToSRV(_In_ wchar_t const* filename, std::wstring texName);
		//int LoadToSRV(wchar_t const* filename, std::wstring texName, _Out_opt_ ID3D11ShaderResourceView** textureView);

		std::shared_ptr<SRV> GetByName(std::wstring name);
	protected:

	private:
		/*Microsoft::WRL::ComPtr<IWICImagingFactory2> m_wicFactory;*/
		ComPtr<ID3D11ShaderResourceView> a;
		int SRVLoadCount;
	};

}