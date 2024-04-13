#include "pch.h"
#include "Model.h"
#include "Graphics.h"

Model::Model()
{
}

Model::~Model()
{
}

std::vector<std::shared_ptr<Mesh>> Model::GetMeshes()
{
	std::vector<std::shared_ptr<Mesh>> meshes;
	for (auto& mesh : meshMap)
	{
		meshes.push_back(mesh.second);
	}
	return meshes;
}

void Model::PrepareRender(Graphics* graphics)
{
	for (auto& mesh : meshMap)
	{
		mesh.second->PrepareRender(graphics);
	}
}

void Model::AddMesh(std::shared_ptr<Mesh> mesh, std::string& meshName)
{
	meshMap.insert({ meshName, mesh });
	meshCount++;
}