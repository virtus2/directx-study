#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class Model
{
public:
	Model();
	~Model();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void AddMaterial(std::shared_ptr<Material> material);
	void PrepareRender(Graphics* graphics);

	std::shared_ptr<Material> GetMaterial() { return material; }
	std::shared_ptr<Mesh> GetMesh(int index) { return meshes[index]; }

private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::shared_ptr<Material> material;
};