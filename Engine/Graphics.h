#pragma once
#include "pch.h"

class Graphics
{
public:
	Graphics();
	~Graphics();

	int Initialize(HWND hWnd, int width, int height);
	void BeginDraw();
	void EndDraw();
	void Clear();
	void Draw();
	void Present();
};

