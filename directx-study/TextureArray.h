#ifndef _TEXTUREARRAY_H_
#define _TEXTUREARRAY_H_

#include <d3d11.h>
#include <DDSTextureLoader.h>

using namespace DirectX;

class TextureArray
{
public:
	TextureArray();
	TextureArray(const TextureArray&);
	~TextureArray();

	bool Initialize(ID3D11Device*, WCHAR*, WCHAR*);
	void Shutdown();

	ID3D11ShaderResourceView** GetTextureArray();

private:
	ID3D11ShaderResourceView* textures[2] = { nullptr, nullptr };

};
#endif