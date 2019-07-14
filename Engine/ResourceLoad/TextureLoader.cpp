#include "pch.h"
#include "TextureLoader.h"
#include "..\Common\DirectXHelper.h"
#include "DDSTextureLoader.h"


using namespace DX;
using namespace Microsoft::WRL;
using namespace DirectX;

TextureLoader::TextureLoader(
	const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice) 
	: BaseLoader(d3dDevice), 
	SRVLoadCount(0)
{
	loader = ref new BasicLoader(d3dDevice.Get());
}

int DX::TextureLoader::LoadToSRV(wchar_t const* filename, std::wstring texName)
{
	auto srv = std::shared_ptr<SRV>(new SRV());
	srv->name = texName;
	loader->LoadTexture(ref new Platform::String(filename), nullptr, &srv->shaderResourceView);
	SRVPool.push_back(srv);
	return SRVLoadCount++;
}

std::shared_ptr<SRV> DX::TextureLoader::GetByName(std::wstring name)
{
	std::vector<std::shared_ptr<SRV>>::iterator it;
	for (it = SRVPool.begin(); it != SRVPool.end(); it++)
	{
		if ((*it)->name == name)
		{
			return (*it);
		}
	}
	return nullptr;
}




