#include "MyGame.h"

// TODO: 엔진 관련 include를 여기에 놓는게 맞을까
#include "Entity.h"
#include "ModelLoader.h"
#include "Camera.h"

#define ZELDA 1
#define RECTANGLE 0
#define CUBE 0

void MyGame::BeginRun()
{
	entity = CreateEntity();

#if ZELDA==1
	// 젤다 테스트 코드
	std::string modelPath = "Models/zeldaPosed001/zeldaPosed001.fbx";
	model = CreateModel(modelPath);
	entity->SetModel(model);
	entity->SetScale(Vector3(0.01f, 0.01f, 0.01f));
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

	camera = CreateCamera();
}

void MyGame::OnUpdate(float deltaTime)
{
	auto input = GetInput();
	/*
	Vector3 cameraVec(camera->GetPosition());
	if (input->IsPressed(Input::Key_A))
	{
		cameraVec += Vector3(-0.1f, 0.0f, 0.0f);
		camera->SetPosition(cameraVec);
	}
	if (input->IsPressed(Input::Key_S))
	{
		cameraVec += Vector3(0.0f, 0.0f, -0.1f);
		camera->SetPosition(cameraVec);
	}
	if (input->IsPressed(Input::Key_D))
	{
		cameraVec += Vector3(0.1f, 0.0f, 0.0f);
		camera->SetPosition(cameraVec);
	}
	if (input->IsPressed(Input::Key_W))
	{
		cameraVec += Vector3(0.0f, 0.0f, 0.1f);
		camera->SetPosition(cameraVec);
	}
	if (input->IsPressed(Input::Key_Space))
	{
		cameraVec += Vector3(0.0f, 0.1f, 0.0f);
		camera->SetPosition(cameraVec);
	}
	if (input->IsPressed(Input::Key_LeftControl))
	{
		cameraVec += Vector3(0.0f, -0.1f, 0.0f);
		camera->SetPosition(cameraVec);
	}
	Utility::Printf("%f %f %f\n", cameraVec.x(), cameraVec.y(), cameraVec.z());
	*/

	Vector3 entityVec(entity->GetPosition());
	if (input->IsPressed(Input::Key_A))
	{
		entityVec += Vector3(-0.1f, 0.0f, 0.0f);
		entity->SetPosition(entityVec);
	}
	if (input->IsPressed(Input::Key_S))
	{
		entityVec += Vector3(0.0f, 0.0f, -0.1f);
		entity->SetPosition(entityVec);
	}
	if (input->IsPressed(Input::Key_D))
	{
		entityVec += Vector3(0.1f, 0.0f, 0.0f);
		entity->SetPosition(entityVec);
	}
	if (input->IsPressed(Input::Key_W))
	{
		entityVec += Vector3(0.0f, 0.0f, 0.1f);
		entity->SetPosition(entityVec);
	}
	if (input->IsPressed(Input::Key_Space))
	{
		entityVec += Vector3(0.0f, 0.1f, 0.0f);
		entity->SetPosition(entityVec);
	}
	if (input->IsPressed(Input::Key_LeftControl))
	{
		entityVec += Vector3(0.0f, -0.1f, 0.0f);
		entity->SetPosition(entityVec);
	}
}
