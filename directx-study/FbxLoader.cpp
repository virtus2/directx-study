#include "FbxLoader.h"

FbxLoader::FbxLoader()
{
	fbxManager = 0;
	fbxIOSettings = 0;
	fbxImporter = 0;
}

FbxLoader::FbxLoader(const FbxLoader&)
{
}

FbxLoader::~FbxLoader()
{
}

bool FbxLoader::Initialize()
{
	fbxManager = FbxManager::Create();
	if(!fbxManager)
	{
		return false;
	}

	fbxIOSettings = FbxIOSettings::Create(fbxManager, IOSROOT);
	fbxManager->SetIOSettings(fbxIOSettings);
	if(!fbxIOSettings)
	{
		return false;
	}
}

void FbxLoader::Shutdown()
{
	fbxManager->Destroy();
	fbxManager = 0;

	fbxIOSettings->Destroy();
	fbxIOSettings = 0;

	fbxImporter->Destroy();
	fbxImporter = 0;
}

bool FbxLoader::LoadFBX(vector<Vertex>& outVertexVector, vector<uint32_t>& outIndexVector, string& filename)
{
	bool result;
	FbxImporter* importer = FbxImporter::Create(fbxManager, "");
	result = importer->Initialize(filename.c_str(), -1, fbxIOSettings);
	if(!result)
	{
		printf("FbxImporter Error: %s\n", importer->GetStatus().GetErrorString());
		return false;
	}

	FbxScene* scene = FbxScene::Create(fbxManager, "tempScene");
	result = importer->Import(scene);
	if(!result)
	{
		return false;
	}
	importer->Destroy();

	if(!scene)
	{
		return false;
	}
	FbxAxisSystem defaultAxisSystem(defaultUpVector, defaultFrontVector, defaultCoordSystem);
	defaultAxisSystem.ConvertScene(scene);

	FbxNode* rootNode = scene->GetRootNode();
	for(int i=0; i<rootNode->GetChildCount(); i++)
	{
		CheckNode(rootNode->GetChild(i));
	}

	int count = scene->GetGeometryCount();
	FbxGeometryConverter converter(fbxManager);
	for(int i=0; i<count; i++)
	{
		FbxGeometry* geometry = scene->GetGeometry(i);
		FbxMesh* mesh;
		if(geometry->Is<FbxMesh>() && ((FbxMesh*)geometry)->IsTriangleMesh())
			mesh = (FbxMesh*)geometry;
		else
		{
			FbxNodeAttribute* attr = converter.Triangulate(geometry, true);
			if (attr->Is<FbxMesh>())
				mesh = (FbxMesh*)attr;
			else
				continue;
		}
		mesh->RemoveBadPolygons();
		int indexCount = mesh->GetPolygonCount() * 3;
		meshMap[geometry] = mesh;
	}

}

void FbxLoader::CheckNode(FbxNode* node)
{
	FbxTransform::EInheritType inheritType;
	node->GetTransformationInheritType(inheritType);
	if(inheritType == FbxTransform::eInheritRrSs)
	{
		node->SetTransformationInheritType(FbxTransform::eInheritRSrs);
	}
	for(int i=0; i<node->GetChildCount(); i++)
	{
		CheckNode(node->GetChild(i));
	}
}