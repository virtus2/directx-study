#include "MyGame.h"

// TODO: 엔진 관련 include를 여기에 놓는게 맞을까
#include "Entity.h"
#include "ModelLoader.h"
#include "Camera.h"

#define PALADIN 0
#define ZELDA 1
#define RECTANGLE 0
#define CUBE 0
#define HELMET 0

void MyGame::BeginRun()
{
	entity = CreateEntity();
#if HELMET==1
	// 헬멧 테스트 코드
	std::string modelPath = "Models/DamagedHelmet.glb";
	model = CreateModel(modelPath);
	entity->SetModel(model);
#endif 

#if PALADIN==1
	// 팔라딘 테스트 코드
	std::string modelPath = "Models/Paladin.fbx";
	model = CreateModel(modelPath);
	entity->SetModel(model);
	entity->SetScale(Vector3(0.1f, 0.1f, 0.1f));
#endif 
#if ZELDA==1
	// 젤다 테스트 코드
	std::string modelPath = "Models/zeldaPosed001/zeldaPosed001.fbx";
	model = CreateModel(modelPath);
	entity->SetModel(model);
	entity->SetScale(XMFLOAT3(0.01f, 0.01f, 0.01f));
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
	auto meshes = model->GetMeshes();
	for (auto& mesh : meshes)
	{
		mesh->GetMaterial()->SetShader(shader);
	}

	camera = CreateCamera();
}

void MyGame::OnUpdate(float deltaTime)
{
	auto input = GetInput();

	// 1인칭 카메라 이동 테스트
	float moveSpeed = 0.005f;
	float turnSpeed = 0.1f;
	float mouseX = input->GetAnalogInput(Input::Mouse_X);
	float mouseY = input->GetAnalogInput(Input::Mouse_Y);
	float yaw = mouseX * DirectX::XM_2PI * turnSpeed * deltaTime;
	float pitch = -mouseY * DirectX::XM_PIDIV2 * turnSpeed * deltaTime;

	XMFLOAT3 pos = camera->GetPosition();
	XMFLOAT3 lookDir = camera->GetLookDirection();
	XMFLOAT3 up = camera->GetUpVector();
	XMFLOAT3 right = camera->GetRightVector();

	XMVECTOR cameraPos = XMLoadFloat3(&pos);
	XMVECTOR cameraLookDir = XMLoadFloat3(&lookDir);
	XMVECTOR upVector = XMLoadFloat3(&up);
	XMVECTOR rightVector = XMLoadFloat3(&right);

	XMFLOAT3 newPos;
	XMMATRIX newLookDir = XMMatrixRotationRollPitchYaw(pitch, yaw, 0);
	cameraLookDir = XMVector3Transform(cameraLookDir, newLookDir);
	if (input->IsPressed(Input::Key_W))
	{
		// 앞으로 이동
		cameraPos += cameraLookDir * deltaTime * moveSpeed;
	}
	if (input->IsPressed(Input::Key_S))
	{
		// 뒤로 이동
		cameraPos -= cameraLookDir * deltaTime * moveSpeed;
	}
	if (input->IsPressed(Input::Key_A))
	{
		// 왼쪽으로 이동
		cameraPos -= rightVector * deltaTime * moveSpeed;
	}
	if (input->IsPressed(Input::Key_D))
	{
		// 오른쪽으로 이동
		cameraPos += rightVector * deltaTime * moveSpeed;
	}

	XMStoreFloat3(&newPos, cameraPos);
	camera->SetPosition(newPos);

	XMStoreFloat3(&lookDir, cameraLookDir);
	camera->SetLookDirection(lookDir);

	Utility::Printf("%f %f\n", lookDir.x, lookDir.y);
}
