#include "Model.h"

Model::Model()
{
	vertexBuffer = 0;
	indexBuffer = 0;
	texture = 0;
}


Model::Model(const Model& other)
{
}


Model::~Model()
{
}

int Model::GetIndexCount()
{
	return indexCount;
}

ID3D11ShaderResourceView* Model::GetTexture()
{
	return texture->GetTexture();
}

ID3D11ShaderResourceView** Model::GetTextureArray()
{
	return textureArray->GetTextureArray();
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename)
{
	bool result;

	texture = new Texture;
	result = texture->Initialize(device, filename);
	if(!result)
	{
		return false;
	}
	return true;
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2)
{
	bool result;
	textureArray = new TextureArray;

	result = textureArray->Initialize(device, filename1, filename2);
	if (!result)
	{
		return false;
	}
	return true;
}

bool Model::LoadTexture(ID3D11Device* device, WCHAR* filename1, WCHAR* filename2, WCHAR* filename3)
{
	bool result;
	textureArray = new TextureArray;

	result = textureArray->Initialize(device, filename1, filename2, filename3);
	if(!result)
	{
		return false;
	}
	return true;
}

void Model::ReleaseTexture()
{
	if(texture)
	{
		texture->Shutdown();
		delete texture;
		texture = 0;
	}
}

bool Model::LoadModel(char* filename)
{
	ifstream fin;
	char input;
	int i;


	// Open the model file.  If it could not open the file then exit.
	fin.open(filename);
	if (fin.fail())
	{
		return false;
	}

	// Read up to the value of vertex count.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}

	// Read in the vertex count.
	fin >> vertexCount;

	// Set the number of indices to be the same as the vertex count.
	indexCount = vertexCount;

	// Create the model using the vertex count that was read in.
	model = new ModelType[vertexCount];
	if (!model)
	{
		return false;
	}

	// Read up to the beginning of the data.
	fin.get(input);
	while (input != ':')
	{
		fin.get(input);
	}
	fin.get(input);
	fin.get(input);

	// Read in the vertex data.
	for (i = 0; i < vertexCount; i++)
	{
		fin >> model[i].x >> model[i].y >> model[i].z;
		fin >> model[i].tu >> model[i].tv;
		fin >> model[i].nx >> model[i].ny >> model[i].nz;
	}

	// Close the model file.
	fin.close();

	return true;
}

void Model::ReleaseModel()
{
	if(model)
	{
		delete model;
		model = 0;
	}
}

bool Model::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename)
{
	bool result;

	// Load in the model data.
	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename);
	if(!result)
	{
		return false;
	}

	return true;
}

bool Model::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2)
{
	bool result;

	result = LoadModel(modelFilename);
	if(!result)
	{
		return false;
	}

	// CalculateModelVectors();

	result = InitializeBuffers(device);
	if(!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename1, textureFilename2);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Model::Initialize(ID3D11Device* device, char* modelFilename, WCHAR* textureFilename1, WCHAR* textureFilename2, WCHAR* textureFilename3)
{
	bool result;

	// Load in the model data.
	result = LoadModel(modelFilename);
	if (!result)
	{
		return false;
	}

	// Initialize the vertex and index buffers.
	result = InitializeBuffers(device);
	if (!result)
	{
		return false;
	}

	result = LoadTexture(device, textureFilename1, textureFilename2, textureFilename3);
	if (!result)
	{
		return false;
	}

	return true;
}

bool Model::InitializeBuffers(ID3D11Device* device)
{
	VertexType* vertices;
	unsigned long* indices;
	D3D11_BUFFER_DESC vertexBufferDesc, indexBufferDesc;
	D3D11_SUBRESOURCE_DATA vertexData, indexData;
	HRESULT result;
	
	// Create the vertex array.
	vertices = new VertexType[vertexCount];
	if (!vertices)
	{
		return false;
	}

	// Create the index array.
	indices = new unsigned long[indexCount];
	if (!indices)
	{
		return false;
	}

	for(int i=0; i<vertexCount; i++)
	{
		vertices[i].position = XMFLOAT3(model[i].x, model[i].y, model[i].z);
		vertices[i].texture = XMFLOAT2(model[i].tu, model[i].tv);
		vertices[i].normal = XMFLOAT3(model[i].nx, model[i].ny, model[i].nz);
		// vertices[i].tangent = XMFLOAT3(model[i].tx, model[i].ty, model[i].tz);
		// vertices[i].binormal = XMFLOAT3(model[i].bx, model[i].by, model[i].bz);
		indices[i] = i;
	}

	// Set up the description of the static vertex buffer.
	vertexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	vertexBufferDesc.ByteWidth = sizeof(VertexType) * vertexCount;
	vertexBufferDesc.BindFlags = D3D11_BIND_VERTEX_BUFFER;
	vertexBufferDesc.CPUAccessFlags = 0;
	vertexBufferDesc.MiscFlags = 0;
	vertexBufferDesc.StructureByteStride = 0;

	// Give the sub-resource structure a pointer to the vertex data.
	vertexData.pSysMem = vertices;
	vertexData.SysMemPitch = 0;
	vertexData.SysMemSlicePitch = 0;

	// Now create the vertex buffer.
	result = device->CreateBuffer(&vertexBufferDesc, &vertexData, &vertexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Set up the description of the static index buffer.
	indexBufferDesc.Usage = D3D11_USAGE_DEFAULT;
	indexBufferDesc.ByteWidth = sizeof(unsigned long) * indexCount;
	indexBufferDesc.BindFlags = D3D11_BIND_INDEX_BUFFER;
	indexBufferDesc.CPUAccessFlags = 0;
	indexBufferDesc.MiscFlags = 0;
	indexBufferDesc.StructureByteStride = 0;

	// Give the subresource structure a pointer to the index data.
	indexData.pSysMem = indices;
	indexData.SysMemPitch = 0;
	indexData.SysMemSlicePitch = 0;

	// Create the index buffer.
	result = device->CreateBuffer(&indexBufferDesc, &indexData, &indexBuffer);
	if (FAILED(result))
	{
		return false;
	}

	// Release the arrays now that the vertex and index buffers have been created and loaded.
	delete[] vertices;
	vertices = 0;

	delete[] indices;
	indices = 0;

	return true;
}

void Model::Shutdown()
{
	// Release the model texture.
	ReleaseTexture();

	// Shutdown the vertex and index buffers.
	ShutdownBuffers();

	// Release the model data.
	ReleaseModel();
}

void Model::ShutdownBuffers()
{
	// Release the index buffer.
	if (indexBuffer)
	{
		indexBuffer->Release();
		indexBuffer = 0;
	}

	// Release the vertex buffer.
	if (vertexBuffer)
	{
		vertexBuffer->Release();
		vertexBuffer = 0;
	}
}

void Model::Render(ID3D11DeviceContext* deviceContext)
{
	// Put the vertex and index buffers on the graphics pipeline to prepare them for drawing.
	RenderBuffers(deviceContext);
}

void Model::RenderBuffers(ID3D11DeviceContext* deviceContext)
{
	unsigned int stride;
	unsigned int offset;

	// Set vertex buffer stride and offset.
	stride = sizeof(VertexType);
	offset = 0;

	// Set the vertex buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetVertexBuffers(0, 1, &vertexBuffer, &stride, &offset);

	// Set the index buffer to active in the input assembler so it can be rendered.
	deviceContext->IASetIndexBuffer(indexBuffer, DXGI_FORMAT_R32_UINT, 0);

	// Set the type of primitive that should be rendered from this vertex buffer, in this case triangles.
	deviceContext->IASetPrimitiveTopology(D3D11_PRIMITIVE_TOPOLOGY_TRIANGLELIST);
}

void Model::CalculateModelVectors()
{
	int faceCount, i, index;
	TempVertexType vertex1, vertex2, vertex3;
	VectorType tangent, binormal, normal;
	
	// Calculate the number of faces in the model.
	faceCount = vertexCount / 3;

	// Initialize the index to the model data.
	index = 0;

	// Go through all the faces and calculate the the tangent, binormal, and normal vectors.
	for (i = 0; i < faceCount; i++)
	{
		// Get the three vertices for this face from the model.
		vertex1.x = model[index].x;
		vertex1.y = model[index].y;
		vertex1.z = model[index].z;
		vertex1.tu = model[index].tu;
		vertex1.tv = model[index].tv;
		vertex1.nx = model[index].nx;
		vertex1.ny = model[index].ny;
		vertex1.nz = model[index].nz;
		index++;

		vertex2.x = model[index].x;
		vertex2.y = model[index].y;
		vertex2.z = model[index].z;
		vertex2.tu = model[index].tu;
		vertex2.tv = model[index].tv;
		vertex2.nx = model[index].nx;
		vertex2.ny = model[index].ny;
		vertex2.nz = model[index].nz;
		index++;

		vertex3.x = model[index].x;
		vertex3.y = model[index].y;
		vertex3.z = model[index].z;
		vertex3.tu = model[index].tu;
		vertex3.tv = model[index].tv;
		vertex3.nx = model[index].nx;
		vertex3.ny = model[index].ny;
		vertex3.nz = model[index].nz;
		index++;

		// Calculate the tangent and binormal of that face.
		CalculateTangentBinormal(vertex1, vertex2, vertex3, tangent, binormal);

		// Calculate the new normal using the tangent and binormal.
		CalculateNormal(tangent, binormal, normal);

		// Store the normal, tangent, and binormal for this face back in the model structure.
		/*
		model[index - 1].nx = normal.x;
		model[index - 1].ny = normal.y;
		model[index - 1].nz = normal.z;
		model[index - 1].tx = tangent.x;
		model[index - 1].ty = tangent.y;
		model[index - 1].tz = tangent.z;
		model[index - 1].bx = binormal.x;
		model[index - 1].by = binormal.y;
		model[index - 1].bz = binormal.z;

		model[index - 2].nx = normal.x;
		model[index - 2].ny = normal.y;
		model[index - 2].nz = normal.z;
		model[index - 2].tx = tangent.x;
		model[index - 2].ty = tangent.y;
		model[index - 2].tz = tangent.z;
		model[index - 2].bx = binormal.x;
		model[index - 2].by = binormal.y;
		model[index - 2].bz = binormal.z;

		model[index - 3].nx = normal.x;
		model[index - 3].ny = normal.y;
		model[index - 3].nz = normal.z;
		model[index - 3].tx = tangent.x;
		model[index - 3].ty = tangent.y;
		model[index - 3].tz = tangent.z;
		model[index - 3].bx = binormal.x;
		model[index - 3].by = binormal.y;
		model[index - 3].bz = binormal.z;
		*/
	}
}

void Model::CalculateTangentBinormal(TempVertexType vertex1, TempVertexType vertex2, TempVertexType vertex3, VectorType& tangent, VectorType& binormal)
{
	float vector1[3], vector2[3];
	float tuVector[2], tvVector[2];
	float den;
	float length;

	// Calculate the two vectors for this face.
	vector1[0] = vertex2.x - vertex1.x;
	vector1[1] = vertex2.y - vertex1.y;
	vector1[2] = vertex2.z - vertex1.z;

	vector2[0] = vertex3.x - vertex1.x;
	vector2[1] = vertex3.y - vertex1.y;
	vector2[2] = vertex3.z - vertex1.z;

	// Calculate the tu and tv texture space vectors.
	tuVector[0] = vertex2.tu - vertex1.tu;
	tvVector[0] = vertex2.tv - vertex1.tv;

	tuVector[1] = vertex3.tu - vertex1.tu;
	tvVector[1] = vertex3.tv - vertex1.tv;

	// Calculate the denominator of the tangent/binormal equation.
	den = 1.0f / (tuVector[0] * tvVector[1] - tuVector[1] * tvVector[0]);

	// Calculate the cross products and multiply by the coefficient to get the tangent and binormal.
	tangent.x = (tvVector[1] * vector1[0] - tvVector[0] * vector2[0]) * den;
	tangent.y = (tvVector[1] * vector1[1] - tvVector[0] * vector2[1]) * den;
	tangent.z = (tvVector[1] * vector1[2] - tvVector[0] * vector2[2]) * den;

	binormal.x = (tuVector[0] * vector2[0] - tuVector[1] * vector1[0]) * den;
	binormal.y = (tuVector[0] * vector2[1] - tuVector[1] * vector1[1]) * den;
	binormal.z = (tuVector[0] * vector2[2] - tuVector[1] * vector1[2]) * den;

	// Calculate the length of this normal.
	length = sqrt((tangent.x * tangent.x) + (tangent.y * tangent.y) + (tangent.z * tangent.z));

	// Normalize the normal and then store it
	tangent.x = tangent.x / length;
	tangent.y = tangent.y / length;
	tangent.z = tangent.z / length;

	// Calculate the length of this normal.
	length = sqrt((binormal.x * binormal.x) + (binormal.y * binormal.y) + (binormal.z * binormal.z));

	// Normalize the normal and then store it
	binormal.x = binormal.x / length;
	binormal.y = binormal.y / length;
	binormal.z = binormal.z / length;
}

void Model::CalculateNormal(VectorType tangent, VectorType binormal, VectorType& normal)
{
	float length;
	
	// Calculate the cross product of the tangent and binormal which will give the normal vector.
	normal.x = (tangent.y * binormal.z) - (tangent.z * binormal.y);
	normal.y = (tangent.z * binormal.x) - (tangent.x * binormal.z);
	normal.z = (tangent.x * binormal.y) - (tangent.y * binormal.x);

	// Calculate the length of the normal.
	length = sqrt((normal.x * normal.x) + (normal.y * normal.y) + (normal.z * normal.z));

	// Normalize the normal.
	normal.x = normal.x / length;
	normal.y = normal.y / length;
	normal.z = normal.z / length;
}
