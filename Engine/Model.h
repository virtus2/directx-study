#pragma once
#include "Mesh.h"
#include "Shader.h"

class Model
{
public:
	Model();
	~Model();

	void AddMesh(std::shared_ptr<Mesh> mesh);
	void AddShader(std::shared_ptr<Shader> shader);
	void PrepareRender(Graphics* graphics);

	std::shared_ptr<Shader> GetShader() { return shader; }
	std::shared_ptr<Mesh> GetMesh(int index) { return meshes[index]; }

private:
	std::vector<std::shared_ptr<Mesh>> meshes;
	std::shared_ptr<Shader> shader;
};