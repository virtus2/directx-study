#include "pch.h"
#include "Mesh.h"

#include "Graphics.h"

Mesh::Mesh()
{
}

Mesh::~Mesh()
{
}

void Mesh::SetMeshData(std::vector<Vertex> vertices, std::vector<uint32_t> indices)
{
	this->vertices = vertices;
	this->indices = indices;
	indexCount = indices.size();
}

void Mesh::PrepareRender(Graphics* graphics)
{
	// 그래픽스 클래스를 통해서 버텍스 버퍼와 인덱스 버퍼를 생성한다.
	graphics->CreateVertexBuffer(vertices, &vertexBuffer);
	graphics->CreateIndexBuffer(indices, &indexBuffer);
}

