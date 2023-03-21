#ifndef _MODEL_H_
#define _MODEL_H_


#include <d3d11.h>
#include <DirectXMath.h>
#include <fstream>

#include "Texture.h"
#include "TextureArray.h"
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
		// XMFLOAT3 tangent;
		// XMFLOAT3 binormal;
	};

	struct ModelType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct TempVertexType
	{
		float x, y, z;
		float tu, tv;
		float nx, ny, nz;
	};

	struct VectorType
	{
		float x, y, z;
	};

public:
	Model();
	Model(const Model&);
	~Model();

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();
	ID3D11ShaderResourceView** GetTextureArray();

	bool Initialize(ID3D11Device*, char*, WCHAR*);
	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*);
	bool Initialize(ID3D11Device*, char*, WCHAR*, WCHAR*, WCHAR*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

private:
	bool LoadTexture(ID3D11Device*, WCHAR*);
	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*);
	bool LoadTexture(ID3D11Device*, WCHAR*, WCHAR*, WCHAR*);
	void ReleaseTexture();
	bool LoadModel(char*);
	void ReleaseModel();
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

	void CalculateModelVectors();
	void CalculateTangentBinormal(TempVertexType, TempVertexType, TempVertexType, VectorType&, VectorType&);
	void CalculateNormal(VectorType, VectorType, VectorType&);

private:
	ID3D11Buffer* vertexBuffer;
	ID3D11Buffer* indexBuffer;
	int vertexCount, indexCount;
	Texture* texture;
	TextureArray* textureArray;
	ModelType* model;
};

#endif