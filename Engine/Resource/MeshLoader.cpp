#include "pch.h"
#include "MeshLoader.h"
#include "BasicShapes.h"
#include "..\Common\DirectXHelper.h"

using namespace Microsoft::WRL;
using namespace DX;

MeshLoader::MeshLoader(const Microsoft::WRL::ComPtr<ID3D11Device3>& d3dDevice):BaseLoader(d3dDevice){}

void MeshLoader::LoadMesh(
	Platform::String ^ filename, 
	ID3D11Buffer ** vertexBuffer, 
	ID3D11Buffer ** indexBuffer, 
	uint32 * vertexCount, 
	uint32 * indexCount,
	uint32* vertexStride
)

{
	Platform::Array<byte>^ meshData = ReadData(filename);

	CreateMesh(
		meshData->Data,
		vertexBuffer,
		indexBuffer,
		vertexCount,
		indexCount,
		vertexStride,
		filename
	);
}
void MeshLoader::CreateMesh(
	byte * meshData, 
	ID3D11Buffer ** vertexBuffer, 
	ID3D11Buffer ** indexBuffer, 
	uint32 * vertexCount, 
	uint32 * indexCount, 
	_Out_opt_ uint32* vertexStride,
	Platform::String ^ debugName)
{
	//顶点数据结构占用bit大小
	auto stride = sizeof(Stride60);
	
	// The first 4 bytes of the BasicMesh format define the number of vertices in the mesh.
	uint32 numVertices = *reinterpret_cast<uint32*>(meshData);

	// The following 4 bytes define the number of indices in the mesh.
	uint32 numIndices = *reinterpret_cast<uint32*>(meshData + sizeof(uint32));

	// The next segment of the BasicMesh format contains the vertices of the mesh.
	Stride60* vertices = reinterpret_cast<Stride60*>(meshData + sizeof(uint32) * 2);

	// The last segment of the BasicMesh format contains the indices of the mesh.
	uint16* indices = reinterpret_cast<uint16*>(meshData + sizeof(uint32) * 2 + stride * numVertices);

	// Create the vertex and index buffers with the mesh data.

	D3D11_SUBRESOURCE_DATA vertexBufferData = { 0 };
	vertexBufferData.pSysMem = vertices;
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC vertexBufferDesc(numVertices * stride, D3D11_BIND_VERTEX_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&vertexBufferDesc,
			&vertexBufferData,
			vertexBuffer
		)
	);

	D3D11_SUBRESOURCE_DATA indexBufferData = { 0 };
	indexBufferData.pSysMem = indices;
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;
	CD3D11_BUFFER_DESC indexBufferDesc(numIndices * sizeof(uint16), D3D11_BIND_INDEX_BUFFER);
	DX::ThrowIfFailed(
		m_d3dDevice->CreateBuffer(
			&indexBufferDesc,
			&indexBufferData,
			indexBuffer
		)
	);
/*
	SetDebugName(*vertexBuffer, Platform::String::Concat(debugName, "_VertexBuffer"));
	SetDebugName(*indexBuffer, Platform::String::Concat(debugName, "_IndexBuffer"));*/

	if (vertexCount != nullptr)
	{
		*vertexCount = numVertices;
	}
	if (indexCount != nullptr)
	{
		*indexCount = numIndices;
	}
	if (vertexStride != nullptr)
	{
		*vertexStride = stride;
	}
}
