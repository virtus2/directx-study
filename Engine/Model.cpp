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
}

void Model::AddShader(std::shared_ptr<Shader> shader)
{
	// TODO: 쉐이더 관리 어떻게 할지 고민
	this->shader = shader;
}

void Model::PrepareRender(Graphics* graphics)
{
	for (auto& mesh : meshes)
	{
		mesh->PrepareRender(graphics);
	}
}