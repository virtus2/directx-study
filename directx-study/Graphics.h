#ifndef _GRAPHICS_H_
#define  _GRAPHICS_H_

#include <windows.h>

#include "AlphaMapShader.h"
#include "D3DClass.h"
#include "Camera.h"
#include "Model.h"
#include "ColorShader.h"
#include "TextureShader.h"
#include "Bitmap.h"
#include "BumpMapShader.h"
#include "LightShader.h"
#include "Light.h"
#include "Text.h"
#include "Frustum.h"
#include "ModelList.h"
#include "MultiTextureShader.h"
#include "LightMapShader.h"
#include "SpecularMapShader.h"
#include "RenderTexture.h"
#include "DebugWindow.h"
#include "FogShader.h"
#include "ClipPlaneShader.h"
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
	bool Frame(int fps, int cpu, float rotationY, int mouseX, int mouseY);

private:
	bool Render(int, int);
	bool RenderToTexture();
	bool RenderScene();

private:
	D3DClass* direct3D;
	Camera* camera;
	Model* model;
	Light* light;
	// ColorShader* colorShader;
	TextureShader* textureShader;
	LightShader* lightShader;
	Bitmap* bitmap;
	Text* text;
	ModelList* modelList;
	Frustum* frustum;
	MultiTextureShader* multiTextureShader;
	LightMapShader* lightMapShader;
	AlphaMapShader* alphaMapShader;
	BumpMapShader* bumpMapShader;
	SpecularMapShader* specularMapShader;
	RenderTexture* renderTexture;
	DebugWindow* debugWindow;
	FogShader* fogShader;
	ClipPlaneShader* clipPlaneShader;
};

#endif