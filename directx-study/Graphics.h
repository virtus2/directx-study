#ifndef _GRAPHICS_H_
#define  _GRAPHICS_H_

#include <windows.h>
#include "D3DClass.h"
#include "FbxLoader.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "LightShader.h"
#include "Light.h"
//
// Globals
//
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 1000.f;
const float SCREEN_NEAR = 0.1f;

class Graphics
{
public:
	Graphics();
	Graphics(const Graphics&);
	~Graphics();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame();

private:
	bool Render(float);

private:
	FbxLoader* fbxLoader;
	D3DClass* direct3D;
	Camera* camera;
	Model* model;
	Light* light;

	ColorShader* colorShader;
	TextureShader* textureShader;
	LightShader* lightShader;
};

#endif