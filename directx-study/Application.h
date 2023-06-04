#ifndef _APPLICATION_H_
#define _APPLICATION_H_


/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.0f;
const float SCREEN_NEAR = 0.1f;


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "Input.h"
#include "d3dclass.h"
#include "Camera.h"
#include "Terrain.h"
#include "ColorShader.h"
#include "Timer.h"
#include "Position.h"
#include "FpsCounter.h"
#include "CpuUsage.h"
#include "FontShader.h"
#include "Text.h"

class Application
{
public:
	Application();
	Application(const Application&);
	~Application();

	bool Initialize(HINSTANCE, HWND, int, int);
	void Shutdown();
	bool Frame();

private:
	bool HandleInput(float);
	bool RenderGraphics();

private:
	Input* input;
	D3DClass* direct3D;
	Camera* camera;
	Terrain* terrain;
	ColorShader* colorShader;
	Timer* timer;
	Position* position;
	FpsCounter* fpsCounter;
	CpuUsage* cpuUsage;
	FontShader* fontShader;
	Text* text;
};

#endif