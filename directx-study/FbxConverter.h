#ifndef _FBXCONVERTER_H_
#define _FBXCONVERTER_H_
#include <DirectXMath.h>
#include <fbxsdk.h>
using namespace DirectX;


class FbxConverter
{
public:
	FbxConverter();
	FbxConverter(const FbxConverter&);
	~FbxConverter();
	
	bool Initialize();
	void Shutdown();
	bool LoadFBX(char* filename);
	bool LoadFBXNode(FbxNode* node);
	bool LoadMesh(FbxNode* node);
	bool LoadSkeleton(FbxNode* node);

private:
	FbxManager* fbxManager;
	FbxIOSettings* fbxIOSettings;
	FbxImporter* fbxImporter;
};

#endif