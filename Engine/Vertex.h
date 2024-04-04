#pragma once
#include "Vector.h"

using namespace Math;

class Vertex 
{
public:
    Vertex(const Vector3& position, const Vector3& normal, const Vector2& texcoord) : position(position), normal(normal), texcoord(texcoord) {}
    Vertex(const Vertex& vertex) : position(vertex.position), normal(vertex.normal), texcoord(vertex.texcoord) {}

    Vector3 position;
    Vector3 normal;
    Vector2 texcoord;
};