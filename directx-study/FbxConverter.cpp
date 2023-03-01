#include "FbxConverter.h"

FbxConverter::FbxConverter()
{
	fbxManager = 0;
	fbxIOSettings = 0;
	fbxImporter = 0;
}

FbxConverter::FbxConverter(const FbxConverter&)
{
}

FbxConverter::~FbxConverter()
{
}

bool FbxConverter::Initialize()
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

void FbxConverter::Shutdown()
{
	fbxManager->Destroy();
	fbxManager = 0;

	fbxIOSettings->Destroy();
	fbxIOSettings = 0;

	fbxImporter->Destroy();
	fbxImporter = 0;
}

bool FbxConverter::LoadFBX(char* filename)
{
	bool result;
	FbxImporter* importer = FbxImporter::Create(fbxManager, "");
	result = importer->Initialize(filename, -1, fbxIOSettings);
	if(!result)
	{
		printf("FbxImporter Error: %s\n", importer->GetStatus().GetErrorString());
		return false;
	}

	FbxScene* scene = FbxScene::Create(fbxManager, "tempScene");
	if(!scene)
	{
		return false;
	}

	result = importer->Import(scene);
	if(!result)
	{
		return false;
	}
	importer->Destroy();

	FbxAxisSystem axis = FbxAxisSystem::eMayaYUp;
	axis.ConvertScene(scene);

	// FbxGeometryConverter* geometryConverter = new FbxGeometryConverter(fbxManager);
	// result = geometryConverter->Triangulate(scene, false);

	FbxNode* rootNode = scene->GetRootNode();
	result = LoadFBXNode(rootNode);
	if(!result)
	{
		return false;
	}
	
	scene->Destroy();
}

bool FbxConverter::LoadFBXNode(FbxNode* node)
{
	bool result;
	FbxNodeAttribute* attr = node->GetNodeAttribute();
	if(attr)
	{
		FbxNodeAttribute::EType type = attr->GetAttributeType();
		if(type == FbxNodeAttribute::eMesh)
		{
			result = LoadMesh(node);
			if(!result)
			{
				return false;
			}
		}
		else if(type == FbxNodeAttribute::eSkeleton)
		{
			result = LoadSkeleton(node);
			if(!result)
			{
				return false;
			}
		}
	}

	int childCount = node->GetChildCount();
	for(int i=0; i<childCount; i++)
	{
		FbxNode* childNode = node->GetChild(i);
		result = LoadFBXNode(childNode);
		if(!result)
		{
			return false;
		}
	}
	return true;
}

bool FbxConverter::LoadMesh(FbxNode* node)
{
	FbxMesh* mesh = node->GetMesh();

	int controlPointsCount = mesh->GetControlPointsCount();
	XMFLOAT3* positions = new XMFLOAT3[controlPointsCount];
	for(int i=0; i<controlPointsCount; i++)
	{
		positions[i].x = static_cast<float>(mesh->GetControlPointAt(i).mData[0]);
		positions[i].y = static_cast<float>(mesh->GetControlPointAt(i).mData[1]);
		positions[i].z = static_cast<float>(mesh->GetControlPointAt(i).mData[2]);
	}

	int polygonCount = mesh->GetPolygonCount();
	int vertexCount = 0;
	for(int i=0; i<polygonCount; i++)
	{
		for(int j=0; j<3; j++)
		{
			int controlPointIndex = mesh->GetPolygonVertex(i, j);
			FbxGeometryElementNormal* vertexNormal = mesh->GetElementNormal(0);
			XMFLOAT3 norm;
			switch(vertexNormal->GetMappingMode())
			{
				case FbxGeometryElement::eByControlPoint:
					switch(vertexNormal->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
							norm.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[0]);
							norm.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[1]);
							norm.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(controlPointIndex).mData[2]);
							break;
						case FbxGeometryElement::eIndexToDirect:
							int index = vertexNormal->GetIndexArray().GetAt(controlPointIndex);
							norm.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
							norm.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
							norm.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
							break;
					}
					break;
				case FbxGeometryElement::eByPolygonVertex:
					switch (vertexNormal->GetReferenceMode())
					{
						case FbxGeometryElement::eDirect:
							norm.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCount).mData[0]);
							norm.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCount).mData[1]);
							norm.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(vertexCount).mData[2]);
							break;
						case FbxGeometryElement::eIndexToDirect:
							int index = vertexNormal->GetIndexArray().GetAt(vertexCount);
							norm.x = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[0]);
							norm.y = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[1]);
							norm.z = static_cast<float>(vertexNormal->GetDirectArray().GetAt(index).mData[2]);
							break;
					}
					break;
			}
			vertexCount++;
		}
	}

	return true;
}

bool FbxConverter::LoadSkeleton(FbxNode* node)
{
	FbxSkeleton* skeleton = node->GetSkeleton();
	return true;
}
