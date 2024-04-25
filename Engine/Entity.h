#pragma once
#include "Matrix.h"
#include <d3d11.h>
class Model;

class Entity
{
	struct ObjectVertexConstantBuffer
	{
		XMMATRIX world;
	};
	static_assert((sizeof(ObjectVertexConstantBuffer) % 16) == 0, "ObjectVertexConstantBuffer size must be 16-byte aligned");
public:
	// TODO: 아직 구조 어떻게 짤 지 안정했음. 추후 수정 필요.
	Entity() {}
	Entity(std::shared_ptr<Model> model) : model(model) {}
	~Entity() {}

	virtual void Update(float deltaTime) {}
	virtual void Render() {}

	// 테스트 코드
	void SetModel(std::shared_ptr<Model> model) { this->model = model; }
	std::shared_ptr<Model> GetModel() { return model; }

	void SetPosition(XMFLOAT3 position) { this->position = position; }
	void SetRotation(XMFLOAT3 rotation) { this->rotation = rotation; }
	void SetScale(XMFLOAT3 scale) { this->scale = scale; }

	XMFLOAT3 GetPosition() { return position; }
	XMFLOAT3 GetRotation() { return rotation; }
	XMFLOAT3 GetScale() { return scale; }

	// TODO: Graphics에서 Getter로 버퍼, 버퍼 데이터 얻어와서 갱신하는 것 vs Entity에서 직접 갱신하는 것 중 어떤 방법이 더 좋은지 고민
	ObjectVertexConstantBuffer* GetObjectVertexConstantBufferData() { return &objectVertexConstantBufferData; }
	ID3D11Buffer* GetObjectVertexConstantBuffer() { return objectVertexConstantBuffer.Get(); }
	size_t GetObjectVertexConstantBufferSize() { return sizeof(ObjectVertexConstantBuffer); }

protected:
	XMFLOAT3 position = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 rotation = XMFLOAT3(0.0f, 0.0f, 0.0f);
	XMFLOAT3 scale = XMFLOAT3(1.0f, 1.0f, 1.0f);

	ObjectVertexConstantBuffer objectVertexConstantBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> objectVertexConstantBuffer = nullptr;

	std::shared_ptr<Model> model;

};