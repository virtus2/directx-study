#pragma once
#include "pch.h"

#include <d3d11.h>
#include "Vertex.h"

class Mesh
{
public:
	Mesh();
	~Mesh();

    void CreateVertexBuffer(ID3D11Device* device, std::vector<Vertex> vertices);
    void CreateIndexBuffer(ID3D11Device* device, std::vector<uint32_t> indices);

private:
    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexConstantBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> pixelConstantBuffer;

    Microsoft::WRL::ComPtr<ID3D11Texture2D> texture;
    Microsoft::WRL::ComPtr<ID3D11ShaderResourceView> textureResourceView;

    unsigned int indexCount = 0;
};
