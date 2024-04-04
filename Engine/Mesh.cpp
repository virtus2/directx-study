#include "pch.h"
#include "Mesh.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::CreateVertexBuffer(ID3D11Device* device, std::vector<Vertex> vertices)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(Vertex) * vertices.size();
	bufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA vertexBufferData = {};
	vertexBufferData.pSysMem = vertices.data();
	vertexBufferData.SysMemPitch = 0;
	vertexBufferData.SysMemSlicePitch = 0;

	const HRESULT result = device->CreateBuffer(&bufferDesc, &vertexBufferData, &vertexBuffer);
	if (FAILED(result)) 
	{
		Utility::Print("Failed to create vertex buffer");
	}
}

void Mesh::CreateIndexBuffer(ID3D11Device* device, std::vector<uint32_t> indices)
{
	D3D11_BUFFER_DESC bufferDesc = {};
	bufferDesc.Usage = D3D11_USAGE_IMMUTABLE;
	bufferDesc.ByteWidth = sizeof(uint32_t) * indices.size();
	bufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	bufferDesc.CPUAccessFlags = 0;
	bufferDesc.MiscFlags = 0;

	D3D11_SUBRESOURCE_DATA indexBufferData = {};
	indexBufferData.pSysMem = indices.data();
	indexBufferData.SysMemPitch = 0;
	indexBufferData.SysMemSlicePitch = 0;

	const HRESULT result = device->CreateBuffer(&bufferDesc, &indexBufferData, &indexBuffer);
	if (FAILED(result))
	{
		Utility::Print("Failed to create index buffer");
	}
}
