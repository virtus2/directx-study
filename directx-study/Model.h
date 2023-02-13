#ifndef _MODEL_H_
#define _MODEL_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>
#include <vector>

#include "FbxLoader.h"
#include "Texture.h"
using namespace std;
using namespace DirectX;

class Model
{
private:
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
		XMFLOAT3 normal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, char*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);
	bool SetFBXModel(ID3D11Device*, std::vector<FbxLoader::Vertex>&);

private:
	bool LoadTexture(ID3D11Device*, ID3D11DeviceContext*, char*);
	void ReleaseTexture();
	bool LoadModel(char*);
	void ReleaseModel();
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;
	Texture* texture;
	ModelType* model;
};

#endif