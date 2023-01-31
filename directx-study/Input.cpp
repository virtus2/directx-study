#include "Input.h"

Input::Input()
{
	
}

Input::Input(const Input&)
{
	
}

Input::~Input()
{
	
}

void Input::Initialize()
{
	for(int i=0; i<256; ++i)
	{
		keys[i] = false;
	}
	return;
}

void Input::KeyDown(unsigned int key)
{
	keys[key] = true;
}

void Input::KeyUp(unsigned int key)
{
	keys[key] = false;
}

bool Input::IsKeyDown(unsigned int key)
{
	return keys[key];
}



