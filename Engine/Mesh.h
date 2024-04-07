#pragma once
#include "pch.h"

#include <d3d11.h>
#include "Vertex.h"

class Graphics;

class Mesh
{
public:
	Mesh();
	~Mesh();

    void SetMeshData(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    void PrepareRender(Graphics* graphics);

    ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer.Get(); }
    ID3D11Buffer* GetIndexBuffer() const { return indexBuffer.Get(); }
    unsigned int GetIndexCount() const { return indexCount; }

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pixelConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;

    unsigned int indexCount = 0;
};