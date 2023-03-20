////////////////////////////////////////////////////////////////////////////////
// Filename: Bitmap.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _BITMAP_H_
#define _BITMAP_H_


//////////////
// INCLUDES //
//////////////
#include <d3d11.h>
#include <DirectXMath.h>


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Texture.h"

using namespace DirectX;
////////////////////////////////////////////////////////////////////////////////
// Class name: Bitmap
////////////////////////////////////////////////////////////////////////////////
class Bitmap
{
	struct VertexType
	{
		XMFLOAT3 position;
		XMFLOAT2 texture;
	};

public:
	Bitmap();
	Bitmap(const Bitmap&);
	~Bitmap();

	bool Initialize(ID3D11Device*, ID3D11DeviceContext*, int, int, WCHAR*, int, int);
	void Shutdown();
	bool Render(ID3D11DeviceContext*, int, int);

	int GetIndexCount();
	ID3D11ShaderResourceView* GetTexture();

private:
	bool InitializeBuffers(ID3D11Device*);
	void ShutdownBuffers();
	bool UpdateBuffers(ID3D11DeviceContext*, int, int);
	void RenderBuffers(ID3D11DeviceContext*);

	bool LoadTexture(ID3D11Device*, WCHAR*);
	void ReleaseTexture();

private:
	ID3D11Buffer* vertexBuffer, *indexBuffer;
	int vertexCount, indexCount;
	Texture* texture;

	int screenWidth, screenHeight;
	int bitmapWidth, bitmapHeight;
	int previousPosX, previousPosY;
};
#endif