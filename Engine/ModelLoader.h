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
	void ProcessNode(const aiScene* scene, const std::shared_ptr<Model> model, aiNode* node);
	void ProcessMesh(const aiScene* scene, const std::shared_ptr<Model> model, aiMesh* mesh);
	void ProcessMaterial(const aiScene* scene, const std::shared_ptr<Model> model, aiMaterial* material, std::string& meshName);
	void ProcessTexture(const aiScene* scene, const std::shared_ptr<Model> model, aiMaterial* material, std::string& meshName, aiTextureType type);

	std::shared_ptr<uint8_t> ProcessEmbeddedTexture(const aiTexture* embeddedTexture, int& outWidth, int& outHeight, int& outChannels);

private:
	Texture::TextureType GetTextureType(aiTextureType type);
	Graphics* graphics = nullptr;
};