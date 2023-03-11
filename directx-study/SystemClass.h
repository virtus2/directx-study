#ifndef _SYSTEMCLASS_H_
#define _SYSTEMCLASS_H_

// Pre-processing directives
#define WIN32_LEAN_AND_MEAN

//
// Includes
//
#include <windows.h>

//
// My Class Includes
//
#include "Input.h"
#include "Graphics.h"
#include "Sound.h"

class SystemClass
{
public:
	SystemClass();
	SystemClass(const SystemClass&);
	~SystemClass();

	bool Initialize();
	void Shutdown();
	void Run();

	LRESULT CALLBACK MessageHandler(HWND, UINT, WPARAM, LPARAM);

private:
	bool Frame();
	void InitializeWindows(int&, int&);
	void ShutdownWindows();

private:
	LPCWSTR applicationName;
	HINSTANCE hInstance;
	HWND hWnd;

	Input* input;
	Graphics* graphics;
	Sound* sound;
};

//
// Function Prototypes
//
static LRESULT CALLBACK WndProc(HWND, UINT, WPARAM, LPARAM);

//
// Globals
//
static SystemClass* ApplicationHandle = 0;

#endif