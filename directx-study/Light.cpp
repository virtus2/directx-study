#include "Light.h"

Light::Light()
{
}


Light::Light(const Light& other)
{
}


Light::~Light()
{
}


void Light::SetDiffuseColor(float red, float green, float blue, float alpha)
{
	diffuseColor = XMFLOAT4(red, green, blue, alpha);
	return;
}


void Light::SetDirection(float x, float y, float z)
{
	direction = XMFLOAT3(x, y, z);
	return;
}


XMFLOAT4 Light::GetDiffuseColor()
{
	return diffuseColor;
}


XMFLOAT3 Light::GetDirection()
{
	return direction;
}