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

void Light::SetAmbientColor(float red, float green, float blue, float alpha)
{
	ambientColor = XMFLOAT4(red, green, blue, alpha);
	return;
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

void Light::SetSpecularColor(float red, float green, float blue, float alpha)
{
	specularColor = XMFLOAT4(red, green, blue, alpha);
	return;
}

void Light::SetSpecularPower(float power)
{
	specularPower = power;
	return;
}

XMFLOAT4 Light::GetSpecularColor()
{
	return specularColor;
}

float Light::GetSpecularPower()
{
	return specularPower;
}

XMFLOAT4 Light::GetAmbientColor()
{
	return ambientColor;
}


XMFLOAT4 Light::GetDiffuseColor()
{
	return diffuseColor;
}


XMFLOAT3 Light::GetDirection()
{
	return direction;
}