#include "pch.h"
#include "ModelLoader.h"
#include "Vertex.h"
#include "Mesh.h"

void ModelLoader::Load(std::string& filePath)
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
			}
		}
	}
}
