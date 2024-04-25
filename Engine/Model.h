#pragma once
#include <map>
#include "Mesh.h"
#include "Shader.h"
#include "Material.h"
struct BoneInfo
{
	XMFLOAT4 boneOffset;
	XMFLOAT4 finalTransform;
	std::string boneName;
};
class Model
{
	friend class ModelLoader;
public:
	Model();
	~Model();

	void PrepareRender(Graphics* graphics);
	void AddMesh(std::string& meshName, std::shared_ptr<Mesh> mesh);

	inline std::vector<std::shared_ptr<Mesh>>& GetMeshes() { return meshList; }
	inline int GetMeshCount() { return meshList.size(); }

private:
	std::string filePath;
	std::map<std::string, std::shared_ptr<Mesh>> meshMap;
	std::vector<std::shared_ptr<Mesh>> meshList;
	
	std::map<std::string, uint32_t> boneMap;
	std::vector<BoneInfo> boneInfo;
};