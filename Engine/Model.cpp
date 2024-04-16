#include "pch.h"
#include "Model.h"
#include "Graphics.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::PrepareRender(Graphics* graphics)
{
	for (auto& mesh : meshMap)
	{
		mesh.second->PrepareRender(graphics);
	}
}

void Model::AddMesh(std::string& meshName, std::shared_ptr<Mesh> mesh)
{
	meshMap.insert({ meshName, mesh });
	meshList.push_back(mesh);
}