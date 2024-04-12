#include "pch.h"
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

void ModelLoader::Load(const std::string& filePath, Model* model)
{
	Assimp::Importer importer;
	const aiScene* scene = importer.ReadFile(filePath.c_str(), aiProcess_Triangulate | aiProcess_ConvertToLeftHanded);

	if (!scene || scene->mFlags & AI_SCENE_FLAGS_INCOMPLETE || !scene->mRootNode)
	{
		Utility::Printf("ModelLoader: Failed to load file %s", filePath);
	}
	else
	{
		if (scene->HasMeshes())
		{
			for (unsigned int i = 0; i < scene->mNumMeshes; i++)
			{
				aiMesh* mesh = scene->mMeshes[i];
				std::vector<Vertex> vertices;
				std::vector<uint32_t> indices;

				for (unsigned int j = 0; j < mesh->mNumVertices; j++)
				{
					Vertex vertex;
					vertex.position = { mesh->mVertices[j].x, mesh->mVertices[j].y, mesh->mVertices[j].z };
					vertex.normal = { mesh->mNormals[j].x, mesh->mNormals[j].y, mesh->mNormals[j].z };

					if (mesh->HasTextureCoords(0))
					{
						vertex.texcoord = { mesh->mTextureCoords[0][j].x, mesh->mTextureCoords[0][j].y };
					}

					vertices.push_back(vertex);
				}

				for (unsigned int j = 0; j < mesh->mNumFaces; j++)
				{
					aiFace face = mesh->mFaces[j];
					for (unsigned int k = 0; k < face.mNumIndices; k++)
					{
						indices.push_back(face.mIndices[k]);
					}
				}

				auto newMesh = std::make_shared<Mesh>();
				newMesh->SetMeshData(vertices, indices);
				model->AddMesh(newMesh);
			}
		}
	}
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
	ProcessNode(scene->mRootNode, scene, model);
	return model;
}

void ModelLoader::ProcessNode(aiNode* node, const aiScene* scene, const std::shared_ptr<Model> model)
{
	for (unsigned int i = 0; i < node->mNumMeshes; i++)
	{
		aiMesh* mesh = scene->mMeshes[node->mMeshes[i]];

		std::shared_ptr<Mesh> meshData = std::make_shared<Mesh>();
		model->AddMesh(meshData);

		std::shared_ptr<Material> material = std::make_shared<Material>();
		model->AddMaterial(material);

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
			vertex.texcoord = { mesh->mTextureCoords[0][i].x, mesh->mTextureCoords[0][i].y };
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
		model->AddMesh(newMesh);
	}

	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	ProcessMaterial(material, model);
}

void ModelLoader::ProcessMaterial(aiMaterial* material, const std::shared_ptr<Model> model)
{
	auto newMaterial = std::make_shared<Material>();
	model->AddMaterial(newMaterial);

	ProcessTexture(material, aiTextureType_DIFFUSE, model);
	ProcessTexture(material, aiTextureType_SPECULAR, model);
	ProcessTexture(material, aiTextureType_AMBIENT, model);
	ProcessTexture(material, aiTextureType_EMISSIVE, model);
	ProcessTexture(material, aiTextureType_HEIGHT, model);
	ProcessTexture(material, aiTextureType_NORMALS, model);
	ProcessTexture(material, aiTextureType_SHININESS, model);
	ProcessTexture(material, aiTextureType_OPACITY, model);
	ProcessTexture(material, aiTextureType_DISPLACEMENT, model);
	ProcessTexture(material, aiTextureType_LIGHTMAP, model);
	ProcessTexture(material, aiTextureType_REFLECTION, model);
	ProcessTexture(material, aiTextureType_BASE_COLOR, model);
	ProcessTexture(material, aiTextureType_NORMAL_CAMERA, model);
	ProcessTexture(material, aiTextureType_EMISSION_COLOR, model);
	ProcessTexture(material, aiTextureType_METALNESS, model);
	ProcessTexture(material, aiTextureType_DIFFUSE_ROUGHNESS, model);
	ProcessTexture(material, aiTextureType_AMBIENT_OCCLUSION, model);
}

void ModelLoader::ProcessTexture(aiMaterial* material, aiTextureType type, const std::shared_ptr<Model> model)
{
	aiString texturePath;
	if (material->GetTextureCount(type) > 0)
	{
		// TODO: type은 다른데 texturePath는 같을 수 있음. 중복해서 로드하지 않게 따로 처리필요함.
		material->GetTexture(type, 0, &texturePath);
		model->GetMaterial()->textureFilePaths.insert({ GetTextureType(type), texturePath.C_Str() });
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
