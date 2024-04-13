#pragma once
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"

class Model
{
	friend class ModelLoader;
public:
	Model();
	~Model();

	void PrepareRender(Graphics* graphics);
	void AddMesh(std::shared_ptr<Mesh> mesh, std::string& meshName);

	std::vector<std::shared_ptr<Mesh>> GetMeshes();
	int GetMeshCount() { return meshCount; }

private:
	std::string filePath;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;
	int meshCount = 0;
};