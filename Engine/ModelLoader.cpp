#include "pch.h"
#include <filesystem>
#include "ModelLoader.h"
#include "Model.h"
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
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);
	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Utility::Printf("ModelLoader: Failed to load file %s", filePath);
		return nullptr;
	}

	auto model = std::make_shared<Model>();
	model->filePath = filePath.substr(0, filePath.find_last_of('/'));
	ProcessNode(scene->mRootNode, scene, model);
	return model;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Model> model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];
		ProcessMesh(mesh, scene, model);
	}
	for (unsigned int i = 0; i < node->mNumChildren; i++)
	{
		ProcessNode(node->mChildren[i], scene, model);
	}
}

void ModelLoader::ProcessMesh(aiMesh* mesh, const aiScene* scene, const std::shared_ptr<Model> model)
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

	newMesh->indexCount = newMesh->indices.size();
	if (newMesh->indexCount > 0)
	{
		auto meshName = std::string(mesh->mName.C_Str());
		model->meshMap.insert({ meshName, newMesh });

		aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
		ProcessMaterial(material, model, meshName);
	}

}

void ModelLoader::ProcessMaterial(aiMaterial* material, const std::shared_ptr<Model> model, std::string& meshName)
{
	auto newMaterial = std::make_shared<Material>();
	model->meshMap[meshName]->material = newMaterial;
	std::string materialName = material->GetName().C_Str();

	ProcessTexture(material, aiTextureType_DIFFUSE, model, meshName);
	ProcessTexture(material, aiTextureType_SPECULAR, model, meshName);
	ProcessTexture(material, aiTextureType_AMBIENT, model, meshName);
	ProcessTexture(material, aiTextureType_EMISSIVE, model, meshName);
	ProcessTexture(material, aiTextureType_HEIGHT, model, meshName);
	ProcessTexture(material, aiTextureType_NORMALS, model, meshName);
	ProcessTexture(material, aiTextureType_SHININESS, model, meshName);
	ProcessTexture(material, aiTextureType_OPACITY, model, meshName);
	ProcessTexture(material, aiTextureType_DISPLACEMENT, model, meshName);
	ProcessTexture(material, aiTextureType_LIGHTMAP, model, meshName);
	ProcessTexture(material, aiTextureType_REFLECTION, model, meshName);
	ProcessTexture(material, aiTextureType_BASE_COLOR, model, meshName);
	ProcessTexture(material, aiTextureType_NORMAL_CAMERA, model, meshName);
	ProcessTexture(material, aiTextureType_EMISSION_COLOR, model, meshName);
	ProcessTexture(material, aiTextureType_METALNESS, model, meshName);
	ProcessTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, model, meshName);
	ProcessTexture(material, aiTextureType_AMBIENT_OCCLUSION, model, meshName);
}

void ModelLoader::ProcessTexture(aiMaterial* material, aiTextureType type, const std::shared_ptr<Model> model, std::string& meshName)
{
	aiString texturePath;
	if (material->GetTextureCount(type) > 0)
	{
		// TODO: type은 다른데 texturePath는 같을 수 있음. 중복해서 로드하지 않게 따로 처리필요함.
		material->GetTexture(type, 0, &texturePath);
		auto modelFilePath = model->filePath;
		auto textureFilePath = std::filesystem::path(texturePath.C_Str()).filename().string();
		std::string textureFullPath = modelFilePath + "/" + textureFilePath;
		// std::string textureFullPath = std::filesystem::path(modelFilePath).append(textureFilePath).string();
		model->meshMap[meshName]->material->textureFilePaths.insert({ GetTextureType(type), textureFullPath });
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
