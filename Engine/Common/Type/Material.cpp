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

void Engine::Material::SetConstantBuffer(std::wstring name, void* sourceData, int32 dataSize, UINT startSlot)
{
	for (std::vector<std::shared_ptr<MateriaCB>>::iterator it = matCBs.begin(); it != matCBs.end(); it++)
	{
		if ((*it)->name == name)
		{
			if ((*it)->dataSize != dataSize)
			{
				(*it)->dataSize = dataSize;
				(*it)->hasCreated = false;
			}
			(*it)->sourceData = sourceData;
			return;
		}
	}
	auto cb = std::shared_ptr<MateriaCB>(new MateriaCB);
	cb->hasCreated = false;
	cb->dataSize = dataSize;
	cb->sourceData = sourceData;
	cb->name = name;
	cb->startSlot = startSlot;
	matCBs.push_back(cb);
}

