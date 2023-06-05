#ifndef _TERRAIN_H_
#define _TERRAIN_H_

#include <d3d11.h>
#include <DirectXMath.h>
#include <cstdio>
using namespace DirectX;
class Terrain
{
private:
	struct VertexType 
	{
		XMFLOAT3 position;
		XMFLOAT4 color;
	};
	struct HeightMapType 
	{
		float x, y, z;
	};
public:
	Terrain();
	Terrain(const Terrain&);
	~Terrain();

	bool Initialize(ID3D11Device*, char*);
	void Shutdown();
	void Render(ID3D11DeviceContext*);

	int GetIndexCount();

private:
	bool LoadHeightMap(char*);
	void NormalizeHeightMap();
	void ShutdownHeightMap();

	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	void RenderBuffers(ID3D11DeviceContext*);

private:
	int terrainWidth, terrainHeight;
	int vertexCount, indexCount;
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	HeightMapType* heightMap;
};

#endif