#pragma once

#include <assimp/Importer.hpp>
#include <assimp/scene.h>
#include <assimp/postprocess.h>
#include <unordered_map>
#include "Texture.h"
class Model;
class Vertex;

class ModelLoader
{
public:
	ModelLoader();
	~ModelLoader();

	int Initialize(Graphics* graphics);

	std::shared_ptr<Model> Load(const std::string& filePath);
	void ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Model> model);
	void ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Model> model);
	void ProcessMaterial(aiMaterial* material, const std::shared_ptr<Model> model, std::string& meshName);
	void ProcessTexture(aiMaterial* material, aiTextureType type, const std::shared_ptr<Model> model, std::string& meshName);

private:
	Texture::TextureType GetTextureType(aiTextureType type);
	Graphics* graphics = nullptr;
};