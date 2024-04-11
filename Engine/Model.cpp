#include "pch.h"
#include "Model.h"
#include "Graphics.h"

Model::Model()
{
}

Model::~Model()
{
}

void Model::AddMesh(std::shared_ptr<Mesh> mesh)
{
	meshes.push_back(mesh);
	meshCount++;
}

void Model::AddMaterial(std::shared_ptr<Material> material)
{
	this->material = material;
}

void Model::PrepareRender(Graphics* graphics)
{
	for (auto& mesh : meshes)
	{
		mesh->PrepareRender(graphics);
	}
}