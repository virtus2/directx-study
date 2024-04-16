#pragma once
#include <d3d11.h>
#include "Vertex.h"
#include "Texture.h"

class Graphics;
class Material;

class Mesh
{
    friend class ModelLoader;
public:
	Mesh();
	~Mesh();

    void SetMeshData(std::vector<Vertex> vertices, std::vector<uint32_t> indices);
    void PrepareRender(Graphics* graphics);

    ID3D11Buffer* GetVertexBuffer() const { return vertexBuffer.Get(); }
    ID3D11Buffer* GetIndexBuffer() const { return indexBuffer.Get(); }
    size_t GetIndexCount() const { return indices.size(); }
    std::shared_ptr<Material> GetMaterial() const { return material; }

private:
    std::vector<Vertex> vertices;
    std::vector<uint32_t> indices;
    std::shared_ptr<Material> material;

    Microsoft::WRL::ComPtr<ID3D11Buffer> vertexBuffer;
    Microsoft::WRL::ComPtr<ID3D11Buffer> indexBuffer;
};