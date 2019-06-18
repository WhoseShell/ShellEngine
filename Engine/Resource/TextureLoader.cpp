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

int DX::TextureLoader::LoadToSRV(wchar_t const* filename, std::wstring matName)
{
	SRVStruct srv;
	allSRV.push_back(srv);
	allSRV[SRVLoadCount].id = SRVLoadCount;
	allSRV[SRVLoadCount].name = matName;
	loader->LoadTexture(ref new Platform::String(filename), nullptr, &allSRV[SRVLoadCount].SRV);
	//LoadTextureOld(filename, allSRV[SRVLoadCount].SRV.Get());
	return SRVLoadCount++;
}


int DX::TextureLoader::LoadToSRV(wchar_t const* filename, std::wstring matName, _Out_opt_ ID3D11ShaderResourceView** textureView)
{
	SRVStruct srv;
	allSRV.push_back(srv);
	allSRV[SRVLoadCount].id = SRVLoadCount;
	allSRV[SRVLoadCount].name = matName;
	loader->LoadTexture(ref new Platform::String(filename), nullptr, textureView);
	//LoadTextureOld(filename, allSRV[SRVLoadCount].SRV.Get());
	return SRVLoadCount++;
}




