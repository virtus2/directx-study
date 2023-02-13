#ifndef _FBXLOADER_H_
#define _FBXLOADER_H_
#include <DirectXMath.h>
#include <fbxsdk.h>
#include <map>
#include <string>
#include <unordered_map>
using namespace std;
using namespace DirectX;


class FbxLoader
{
public:
	struct Vertex
	{
		XMFLOAT3 position;
		XMFLOAT3 normal;
		XMFLOAT2 uv;
	};
	FbxLoader();
	FbxLoader(const FbxLoader&);
	~FbxLoader();
	
	bool Initialize();
	void Shutdown();
	bool LoadFBX(vector<Vertex>& outVertexVector, vector<uint32_t>& outIndexVector, string& filename);
	void CheckNode(FbxNode* node);
	vector<Vertex> GetVertices(char* filename) { return loadedVertices[filename]; }

public:
	static const FbxAxisSystem::EUpVector defaultUpVector = FbxAxisSystem::eYAxis;
	static const FbxAxisSystem::EFrontVector defaultFrontVector = FbxAxisSystem::eParityOdd;
	static const FbxAxisSystem::ECoordSystem defaultCoordSystem = FbxAxisSystem::eRightHanded;
private:
	FbxManager* fbxManager;
	FbxIOSettings* fbxIOSettings;
	FbxImporter* fbxImporter;

	unordered_map<std::string, std::vector<Vertex>> loadedVertices;
	map<FbxGeometry*, FbxMesh*> meshMap;
};

#endif