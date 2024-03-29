#ifndef _TEXTURE_H_
#define _TEXTURE_H_


//////////////
// INCLUDES //
//////////////
#pragma comment(lib, "DirectXTK.lib")
#include <d3d11.h>
#include <stdio.h>
#include <DDSTextureLoader.h>
using namespace DirectX;
class Texture
{
private:
	struct TargaHeader
	{
		unsigned char data1[12];
		unsigned short width;
		unsigned short height;
		unsigned char bpp;
		unsigned char data2;
	};

public:
	Texture();
	Texture(const Texture&);
	~Texture();

	bool Initialize(ID3D11Device*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView* GetTexture();

private:
	bool LoadTarga(char*, int&, int&);

private:
	unsigned char* targaData;
	ID3D11Resource* texture;
	ID3D11ShaderResourceView* textureView;


};
#endif