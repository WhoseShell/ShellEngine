#include"pch.h"
#include"Material.h"

void Engine::Material::SetTexture(ComPtr<ID3D11ShaderResourceView>& srv)
{
	for (std::vector<ComPtr<ID3D11ShaderResourceView>>::iterator it = SRVs.begin(); it != SRVs.end(); it++)
	{
		if ((*it) == srv)
			return;
	}
	SRVs.push_back(srv);
}

