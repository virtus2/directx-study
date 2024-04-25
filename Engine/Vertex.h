#pragma once
#include "Vector.h"

using namespace Math;

class Vertex 
{
public:
    Vertex() : position(XMFLOAT3(0.0f, 0.0f, 0.0f)), normal(XMFLOAT3(0.0f, 0.0f, 0.0f)), texcoord(XMFLOAT2(0.0f, 0.0f)), 
        boneWeights(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)), boneIndices(XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f)) {}
    Vertex(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT2& texcoord) : position(position), normal(normal), texcoord(texcoord) {}
    Vertex(const Vertex& vertex) : position(vertex.position), normal(vertex.normal), texcoord(vertex.texcoord) {}

    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;

    // TODO: 뼈대 데이터 없는 Vertex 추가 (StaticMesh)
    XMFLOAT4 boneWeights = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
    XMFLOAT4 boneIndices = XMFLOAT4(0.0f, 0.0f, 0.0f, 0.0f);
};