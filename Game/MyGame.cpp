#include "MyGame.h"

// TODO: 엔진 관련 include를 여기에 놓는게 맞을까
#include "Entity.h"
#include "ModelLoader.h"
#define ZELDA 0
#define RECTANGLE 0
#define CUBE 1

void MyGame::BeginRun()
{
	entity = CreateEntity();

#if ZELDA==1
	// 젤다 테스트 코드
	std::string modelPath = "Models/zeldaPosed001/zeldaPosed001.fbx";
	model = CreateModel(modelPath);
	entity->SetModel(model);
#endif
 
#if RECTANGLE==1
	//네모 만들기 테스트 코드 
	model = CreateRectangle();
	entity->SetModel(model);
#endif

#if CUBE==1
	//네모 만들기 테스트 코드 
	model = CreateCube();
	entity->SetModel(model);
#endif

	const std::wstring vertexShaderPath = L"Shaders/SimpleVertexShader.hlsl";
	const std::wstring pixelShaderPath = L"Shaders/SimplePixelShader.hlsl";
	shader = CreateShader(vertexShaderPath, pixelShaderPath);
	
	material = CreateMaterial();
	material->SetShader(shader);

	model->AddMaterial(material);
}

void MyGame::OnUpdate()
{
	Utility::Printf("index count: %d\n", entity->GetModel()->GetMesh(0)->GetIndexCount());
}
