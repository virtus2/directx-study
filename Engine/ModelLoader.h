#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>

class Model;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	int Initialize(Graphics* graphics);
	void Load(const std::string& filePath, Model* model);

private:
	Graphics* graphics = nullptr;
};