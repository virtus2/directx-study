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
	void AddMesh(std::string& meshName, std::shared_ptr<Mesh> mesh);

	inline std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return meshList; }
	inline int GetMeshCount() { return meshList.size(); }

private:
	std::string filePath;
	std::unordered_map<std::string, std::shared_ptr<Mesh>> meshMap;
	std::vector<std::shared_ptr<Mesh>> meshList;
};