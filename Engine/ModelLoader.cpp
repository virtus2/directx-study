#include "pch.h"
#include <filesystem>
#include "ModelLoader.h"
#include "Model.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

ModelLoader::ModelLoader()
{
}

ModelLoader::~ModelLoader()
{
}

int ModelLoader::Initialize(Graphics* graphics)
{
	this->graphics = graphics;
	return 0;
}

std::shared_ptr<Model> ModelLoader::Load(const std::string& filePath)
{
	Assimp::Importer importer;
	importer.SetPropertyBool(AI_CONFIG_IMPORT_FBX_PRESERVE_PIVOTS, false);
	const unsigned int readFileFlags =
		aiProcess_Triangulate |
		aiProcess_SortByPType | // ?
		aiProcess_GenUVCoords |
		aiProcess_OptimizeMeshes |
		aiProcess_CalcTangentSpace |
		aiProcess_LimitBoneWeights | // ?
		aiProcess_JoinIdenticalVertices
		;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), readFileFlags);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Utility::Printf("ModelLoader: Failed to load file %s", filePath);
		return nullptr;
	}

	auto model = std::make_shared<Model>();
	model->filePath = filePath.substr(0, filePath.find_last_of('/'));
	ProcessNode(scene, model, scene->mRootNode);
	return model;
}

void ModelLoader::ProcessNode(const aiScene* scene, const std::shared_ptr<Model> model, aiNode* node)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(scene, model, mesh);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(scene, model, node->mChildren[i]);
	}
}

void ModelLoader::ProcessMesh(const aiScene* scene, const std::shared_ptr<Model> model, aiMesh* mesh)
{
	auto newMesh = std::make_shared<Mesh>();
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };

		if (mesh->HasTextureCoords(0))
		{
			vertex.texcoord = { (float)mesh->mTextureCoords[0][i].x, (float)mesh->mTextureCoords[0][i].y };
		}
		else
		{
			vertex.texcoord = { 0.0f, 0.0f };
		}

		// TODO: tangent, bitangent...

		newMesh->vertices.push_back(vertex);
	}

	for (unsigned int i = 0; i < mesh->mNumFaces; i++)
	{
		aiFace face = mesh->mFaces[i];
		for (unsigned int j = 0; j < face.mNumIndices; j++)
		{
			newMesh->indices.push_back(face.mIndices[j]);
		}
	}

	auto meshName = std::string(mesh->mName.C_Str());
	model->AddMesh(meshName, newMesh);
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	ProcessMaterial(scene, model, material, meshName);
}

void ModelLoader::ProcessMaterial(const aiScene* scene, const std::shared_ptr<Model> model, aiMaterial* material, std::string& meshName)
{
	auto newMaterial = std::make_shared<Material>();
	model->meshMap[meshName]->material = newMaterial;
	std::string materialName = material->GetName().C_Str();

	aiColor4D diffuse;
	if (material->Get(AI_MATKEY_COLOR_DIFFUSE, diffuse))
	{
		newMaterial->diffuse = { diffuse.r, diffuse.g, diffuse.b, diffuse.a };
	}

	aiColor4D specular;
	if (material->Get(AI_MATKEY_COLOR_SPECULAR, specular))
	{
		newMaterial->specular = { specular.r, specular.g, specular.b, specular.a };
	}

	aiColor4D ambient;
	if (material->Get(AI_MATKEY_COLOR_AMBIENT, ambient))
	{
		newMaterial->ambient = { ambient.r, ambient.g, ambient.b, ambient.a };
	}

	/*
	if (scene->HasTextures())
	{
		for (int i = 0; i < scene->mNumTextures; i++)
		{
			auto texture = scene->mTextures[i];
			int width, height, channels;
			uint8_t* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData), texture->mWidth, &width, &height, &channels, STBI_rgb_alpha);
			auto newTexture = std::make_shared<Texture>();
			newTexture->CreateTexture(graphics, data, width, height);
			// TODO: 매테리얼에 텍스처 추가하는 코드 리팩토링
			model->meshMap[meshName]->material->textures.insert({ GetTextureType(aiTextureType_DIFFUSE), newTexture });
		}
	}*/

	ProcessTexture(scene, model, material, meshName, aiTextureType_DIFFUSE);
	ProcessTexture(scene, model, material, meshName, aiTextureType_SPECULAR);
	ProcessTexture(scene, model, material, meshName, aiTextureType_AMBIENT);
	ProcessTexture(scene, model, material, meshName, aiTextureType_EMISSIVE);
	ProcessTexture(scene, model, material, meshName, aiTextureType_HEIGHT);
	ProcessTexture(scene, model, material, meshName, aiTextureType_NORMALS);
	ProcessTexture(scene, model, material, meshName, aiTextureType_SHININESS);
	ProcessTexture(scene, model, material, meshName, aiTextureType_OPACITY);
	ProcessTexture(scene, model, material, meshName, aiTextureType_DISPLACEMENT);
	ProcessTexture(scene, model, material, meshName, aiTextureType_LIGHTMAP);
	ProcessTexture(scene, model, material, meshName, aiTextureType_REFLECTION);
	ProcessTexture(scene, model, material, meshName, aiTextureType_BASE_COLOR);
	ProcessTexture(scene, model, material, meshName, aiTextureType_NORMAL_CAMERA);
	ProcessTexture(scene, model, material, meshName, aiTextureType_EMISSION_COLOR);
	ProcessTexture(scene, model, material, meshName, aiTextureType_METALNESS);
	ProcessTexture(scene, model, material, meshName, aiTextureType_DIFFUSE_ROUGHNESS);
	ProcessTexture(scene, model, material, meshName, aiTextureType_AMBIENT_OCCLUSION);
}

void ModelLoader::ProcessTexture(const aiScene* scene, const std::shared_ptr<Model> model, aiMaterial* material, std::string& meshName, aiTextureType type)
{
	if (material->GetTextureCount(type) > 0)
	{
		aiString path;
		material->GetTexture(type, 0, &path);
		std::string texturePath(path.C_Str());
		if (texturePath[0] == '*')
		{
			/*
			* If the texture is embedded, receives a '*' followed by the id of
			* the texture(for the textures stored in the corresponding scene) which
			* can be converted to an int using a function like atoi.
			* This parameter must be non - null.
			*/
			int textureId = std::atoi(texturePath.substr(1, texturePath.size()).c_str());
			aiTexture* texture = scene->mTextures[textureId];
			int width, height, channels;
			uint8_t* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(texture->pcData), texture->mWidth, &width, &height, &channels, STBI_rgb_alpha);
			auto newTexture = std::make_shared<Texture>();
			newTexture->CreateTexture(graphics, data, width, height);
			// TODO: 매테리얼에 텍스처 추가하는 코드 리팩토링
			model->meshMap[meshName]->material->textures.insert({ GetTextureType(type), newTexture });
		}
		else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.c_str()))
		{
			int width, height, channels;
			uint8_t* data = stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(embeddedTexture->pcData), embeddedTexture->mWidth, &width, &height, &channels, STBI_rgb_alpha);
			auto newTexture = std::make_shared<Texture>();
			newTexture->CreateTexture(graphics, data, width, height);
			// TODO: 매테리얼에 텍스처 추가하는 코드 리팩토링
			model->meshMap[meshName]->material->textures.insert({ GetTextureType(type), newTexture });
		}
		else
		{
			auto modelFilePath = model->filePath;
			auto textureFilePath = std::filesystem::path(texturePath).filename().string();
			std::string fullPath = modelFilePath + "/" + textureFilePath;
			// TODO: 매테리얼에 텍스처 추가하는 코드 리팩토링
			model->meshMap[meshName]->material->textureFilePaths.insert({ GetTextureType(type), fullPath });
		}
	}
}


Texture::TextureType ModelLoader::GetTextureType(aiTextureType type)
{
	switch (type)
	{
		case aiTextureType_DIFFUSE:
			return Texture::TextureType::Diffuse;
		case aiTextureType_SPECULAR:
			return Texture::TextureType::Specular;
		case aiTextureType_AMBIENT:
			return Texture::TextureType::Ambient;
		case aiTextureType_EMISSIVE:
			return Texture::TextureType::Emissive;
		case aiTextureType_HEIGHT:
			return Texture::TextureType::Height;
		case aiTextureType_NORMALS:
			return Texture::TextureType::Normals;
		case aiTextureType_SHININESS:
			return Texture::TextureType::Shininess;
		case aiTextureType_OPACITY:
			return Texture::TextureType::Opacity;
		case aiTextureType_DISPLACEMENT:
			return Texture::TextureType::Displacement;
		case aiTextureType_LIGHTMAP:
			return Texture::TextureType::Lightmap;
		case aiTextureType_REFLECTION:
			return Texture::TextureType::Reflection;
		case aiTextureType_BASE_COLOR:
			return Texture::TextureType::BaseColor;
		case aiTextureType_NORMAL_CAMERA:
			return Texture::TextureType::NormalCamera;
		case aiTextureType_EMISSION_COLOR:
			return Texture::TextureType::EmissionColor;
		case aiTextureType_METALNESS:
			return Texture::TextureType::Metalness;
		case aiTextureType_DIFFUSE_ROUGHNESS:
			return Texture::TextureType::DiffuseRoughness;
		case aiTextureType_AMBIENT_OCCLUSION:
			return Texture::TextureType::AmbientOcclusion;
		default:
			return Texture::TextureType::Count;
	}
}
