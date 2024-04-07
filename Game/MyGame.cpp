#include "MyGame.h"

// TODO: 엔진 관련 include를 여기에 놓는게 맞을까
#include "Entity.h"
#include "ModelLoader.h"

void MyGame::BeginRun()
{
	entity = CreateEntity();

	std::string modelPath = "Models/zeldaPosed001/zeldaPosed001.fbx";
	auto model = CreateModel(modelPath);
	entity->SetModel(model);
	/*
	* 네모 만들기 테스트 코드
	auto model = CreateRectangle();
	entity->SetModel(model);
	*/
	const std::wstring vertexShaderPath = L"Shaders/SimpleVertexShader.hlsl";
	const std::wstring pixelShaderPath = L"Shaders/SimplePixelShader.hlsl";
	auto shader = CreateShader(vertexShaderPath, pixelShaderPath);
	model->AddShader(shader);
}

void MyGame::OnUpdate()
{
	Utility::Printf("index count: %d\n", entity->GetModel()->GetMesh(0)->GetIndexCount());
}
