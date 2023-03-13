#include "TextureArray.h"

TextureArray::TextureArray()
{
}

TextureArray::TextureArray(const TextureArray&)
{
}

TextureArray::~TextureArray()
{
}

bool TextureArray::Initialize(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	HRESULT result;
	result = CreateDDSTextureFromFile(device, filename1, nullptr, &textures[0]);
	if(FAILED(result))
	{
		return false;
	}

	result = CreateDDSTextureFromFile(device, filename2, nullptr, &textures[1]);
	if(FAILED(result))
	{
		return false;
	}

	return true;
}

void TextureArray::Shutdown()
{
	if(textures[0])
	{
		textures[0]->Release();
		textures[0] = 0;
	}

	if (textures[1])
	{
		textures[1]->Release();
		textures[1] = 0;
	}
}

ID3D11ShaderResourceView** TextureArray::GetTextureArray()
{
	return textures;
}
