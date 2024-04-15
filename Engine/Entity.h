#pragma once
#include "Matrix.h"
#include <d3d11.h>
class Model;

class Entity
{
	struct ObjectVertexConstantBuffer
	{
		Math::Matrix world;
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

	void SetPosition(Math::Vector3 position) { this->position = position; }
	void SetRotation(Math::Vector3 rotation) { this->rotation = rotation; }
	void SetScale(Math::Vector3 scale) { this->scale = scale; }

	Math::Vector3 GetPosition() { return position; }
	Math::Vector3 GetRotation() { return rotation; }
	Math::Vector3 GetScale() { return scale; }

	// TODO: Graphics에서 Getter로 버퍼, 버퍼 데이터 얻어와서 갱신하는 것 vs Entity에서 직접 갱신하는 것 중 어떤 방법이 더 좋은지 고민
	ObjectVertexConstantBuffer* GetObjectVertexConstantBufferData() { return &objectVertexConstantBufferData; }
	ID3D11Buffer* GetObjectVertexConstantBuffer() { return objectVertexConstantBuffer.Get(); }
	size_t GetObjectVertexConstantBufferSize() { return sizeof(ObjectVertexConstantBuffer); }

protected:
	Math::Vector3 position = Math::Vector3::Zero();
	Math::Vector3 rotation = Math::Vector3::Zero();
	Math::Vector3 scale = Math::Vector3::One();

	ObjectVertexConstantBuffer objectVertexConstantBufferData;
	Microsoft::WRL::ComPtr<ID3D11Buffer> objectVertexConstantBuffer = nullptr;

	std::shared_ptr<Model> model;

};