#pragma once
#include "Vector.h"

using namespace Math;

class Vertex 
{
public:
    Vertex() : position(XMFLOAT3()), normal(XMFLOAT3()), texcoord(XMFLOAT2()) {}
    Vertex(const XMFLOAT3& position, const XMFLOAT3& normal, const XMFLOAT2& texcoord) : position(position), normal(normal), texcoord(texcoord) {}
    Vertex(const Vertex& vertex) : position(vertex.position), normal(vertex.normal), texcoord(vertex.texcoord) {}

    XMFLOAT3 position;
    XMFLOAT3 normal;
    XMFLOAT2 texcoord;
};