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
		aiProcess_JoinIdenticalVertices |
		aiProcess_ConvertToLeftHanded
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

	if (mesh->HasBones())
	{
		for (unsigned int i = 0; i < mesh->mNumBones; i++)
		{
			aiBone* bone = mesh->mBones[i];
			std::string boneName(bone->mName.C_Str());

			if (model->boneMap.find(boneName) == model->boneMap.end())
			{
				int newBoneIndex = model->boneMap.size();
				BoneInfo boneInfo;
				boneInfo.boneName = boneName;
				// TODO: aiMatrix를 XMMATRIX로
				// boneInfo.boneOffset = bone->mOffsetMatrix;
				model->boneInfo.push_back(boneInfo);
				model->boneMap[boneName] = newBoneIndex;
			}
		}
	}

	// TODO: 뼈대 없으면 뼈대 데이터 없는 Vertex 사용
	for (unsigned int i = 0; i < mesh->mNumVertices; i++)
	{
		Vertex vertex;
		
		if (mesh->HasPositions())
		{
			vertex.position = { mesh->mVertices[i].x, mesh->mVertices[i].y, mesh->mVertices[i].z };
		}
		if (mesh->HasNormals())
		{
			vertex.normal = { mesh->mNormals[i].x, mesh->mNormals[i].y, mesh->mNormals[i].z };
		}
		if (mesh->HasTextureCoords(0))
		{
			vertex.texcoord = { (float)mesh->mTextureCoords[0][i].x, (float)mesh->mTextureCoords[0][i].y };
		}
		if (mesh->HasTangentsAndBitangents())
		{
			// TODO: tangent, bitangent...
		}
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

	for (unsigned int i = 0; i < mesh->mNumBones; i++)
	{
		for (unsigned int j = 0; j < mesh->mBones[i]->mNumWeights; j++)
		{
			std::string boneName(mesh->mBones[i]->mName.data);
			unsigned int boneIndex = model->boneMap[boneName];
			unsigned int vertexIndex = mesh->mBones[i]->mWeights[j].mVertexId;
			float weight = mesh->mBones[i]->mWeights[j].mWeight;

			auto& vertex = newMesh->vertices[vertexIndex];
			if (vertex.boneWeights.x == 0)
			{
				vertex.boneIndices.x = boneIndex;
				vertex.boneWeights.x = weight;
			}
			else if (vertex.boneWeights.y == 0)
			{
				vertex.boneIndices.y = boneIndex;
				vertex.boneWeights.y = weight;
			}
			else if (vertex.boneWeights.z == 0)
			{
				vertex.boneIndices.z = boneIndex;
				vertex.boneWeights.z = weight;
			}
			else if (vertex.boneWeights.w == 0)
			{
				vertex.boneIndices.w = boneIndex;
				vertex.boneWeights.w = weight;
			}
		}
	}

	auto meshName = std::string(mesh->mName.data);
	model->AddMesh(meshName, newMesh);
	aiMaterial* material = scene->mMaterials[mesh->mMaterialIndex];
	ProcessMaterial(scene, model, material, meshName);
}

void ModelLoader::ProcessMaterial(const aiScene* scene, const std::shared_ptr<Model> model, aiMaterial* material, std::string& meshName)
{
	auto newMaterial = std::make_shared<Material>();
	model->meshMap[meshName]->material = newMaterial;
	std::string materialName(material->GetName().data);

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
		std::string texturePath(path.data);

		int width = 0, height = 0, channels = 0;
		auto newTexture = std::make_shared<Texture>();
		if (texturePath[0] == '*')
		{
			// texturePath가 '*0', '*1', ... 일 경우 임베디드 텍스처로, 해당 씬의 텍스처 ID를 뜻하는 것이다.
			// 해당 번호로 씬에서 텍스처를 참조한다.

			int textureId = std::atoi(texturePath.substr(1, texturePath.size()).c_str());
			aiTexture* texture = scene->mTextures[textureId];
			auto data = ProcessEmbeddedTexture(texture, width, height, channels);
			newTexture->CreateTexture(graphics, data.get(), width, height);
		}
		else if (auto embeddedTexture = scene->GetEmbeddedTexture(texturePath.c_str()))
		{
			// 위의 경우가 아님에도 임베디드 텍스처인 경우가 있다...(ex: Mixamo에서 가져온 fbx파일)
			// 그럴땐 scene->GetEmbeddedTexture에다 경로를 그대로 넘겨주면 된다.

			auto data = ProcessEmbeddedTexture(embeddedTexture, width, height, channels);
			newTexture->CreateTexture(graphics, data.get(), width, height);
		}
		else
		{
			// 텍스처 파일이 외부에 따로 존재하는 경우, 해당 경로에 있는 파일을 불러와준다.

			auto modelFilePath = model->filePath;
			auto textureFilePath = std::filesystem::path(texturePath).filename().string();
			std::string fullPath = modelFilePath + "/" + textureFilePath;
			newTexture->CreateTexture(graphics, Utility::UTF8ToWideString(fullPath));
		}

		// TODO: 매테리얼에 텍스처 추가하는 코드 리팩토링
		model->meshMap[meshName]->material->textures.insert({ GetTextureType(type), newTexture });
		// model->meshMap[meshName]->material->textureFilePaths.insert({ GetTextureType(type), fullPath });
	}
}

std::shared_ptr<uint8_t> ModelLoader::ProcessEmbeddedTexture(const aiTexture* embeddedTexture, int& outWidth, int& outHeight, int& outChannels)
{
	// uint8_t** 말고 std::vector<uint8_t>&은 어떨까...
	// 텍스처의 mHeight가 0일 경우 압축된 텍스처(ex: JPEG)이다.
	if (embeddedTexture->mHeight == 0)
	{
		std::shared_ptr<uint8_t> data(stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(embeddedTexture->pcData), embeddedTexture->mWidth, &outWidth, &outHeight, &outChannels,
			STBI_rgb_alpha));
		return data;
	}
	else
	{
		std::shared_ptr<uint8_t> data(stbi_load_from_memory(reinterpret_cast<const stbi_uc*>(embeddedTexture->pcData), embeddedTexture->mWidth, &outWidth, &outHeight, &outChannels,
			STBI_rgb_alpha));
		return data;
	}
	// 매개변수에 STBI_rgb_alpha를 넘겨줬으니 
	// data[0] = r, 
	// data[1] = g, 
	// data[2] = b, 
	// data[3] = a
	// 이런식으로 나온다. 
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
