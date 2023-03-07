#include "Font.h"

Font::Font()
{
	font = 0;
	texture = 0;
}

Font::Font(const Font&)
{
}

Font::~Font()
{
}

bool Font::Initialize(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* fontFilename, char* textureFilename)
{
	bool result;

	result = LoadFontData(fontFilename);
	if(!result)
	{
		return false;
	}

	result = LoadTexture(device, deviceContext, textureFilename);
	if(!result)
	{
		return false;
	}
	return true;
}

void Font::Shutdown()
{
	ReleaseTexture();
	ReleaseFontData();
}

ID3D11ShaderResourceView* Font::GetTexture()
{
	return texture->GetTexture();
}

void Font::BuildVertexArray(void* vertices, char* sentence, float drawX, float drawY)
{
	VertexType* vertexPtr;
	int numLetters, index, letter;

	// Coerce the input vertices into a VertexType structure.
	vertexPtr = (VertexType*)vertices;

	// Get the number of letters in the sentence.
	numLetters = (int)strlen(sentence);

	// Initialize the index to the vertex array.
	index = 0;
	// Draw each letter onto a quad.
	for (int i = 0; i < numLetters; i++)
	{
		letter = ((int)sentence[i]) - 32;

		// If the letter is a space then just move over three pixels.
		if (letter == 0)
		{
			drawX = drawX + 3.0f;
		}
		else
		{
			// First triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(font[letter].right, 1.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX, (drawY - 16), 0.0f);  // Bottom left.
			vertexPtr[index].texture = XMFLOAT2(font[letter].left, 1.0f);
			index++;

			// Second triangle in quad.
			vertexPtr[index].position = XMFLOAT3(drawX, drawY, 0.0f);  // Top left.
			vertexPtr[index].texture = XMFLOAT2(font[letter].left, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3(drawX + font[letter].size, drawY, 0.0f);  // Top right.
			vertexPtr[index].texture = XMFLOAT2(font[letter].right, 0.0f);
			index++;

			vertexPtr[index].position = XMFLOAT3((drawX + font[letter].size), (drawY - 16), 0.0f);  // Bottom right.
			vertexPtr[index].texture = XMFLOAT2(font[letter].right, 1.0f);
			index++;

			// Update the x location for drawing by the size of the letter and one pixel.
			drawX = drawX + font[letter].size + 1.0f;
		}
	}
}

bool Font::LoadFontData(char* filename)
{
	ifstream fin;
	char temp;

	font = new FontType[95];
	if(!font)
	{
		return false;
	}

	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read in the 95 used ascii characters for text.
	for (int i = 0; i < 95; i++)
	{
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}
		fin.get(temp);
		while (temp != ' ')
		{
			fin.get(temp);
		}

		fin >> font[i].left;
		fin >> font[i].right;
		fin >> font[i].size;
	}

	// Close the file.
	fin.close();

	return true;
}

void Font::ReleaseFontData()
{
	if(font)
	{
		delete[] font;
		font = 0;
	}
}

bool Font::LoadTexture(ID3D11Device* device, ID3D11DeviceContext* deviceContext, char* filename)
{
	bool result;

	texture = new Texture;
	if(!texture)
	{
		return false;
	}

	result = texture->Initialize(device, deviceContext, filename);
	if (!result)
	{
		return false;
	}

	return true;
}

void Font::ReleaseTexture()
{
	if(texture)
	{
		texture->Shutdown();
		delete texture;
		texture = 0;
	}
}
