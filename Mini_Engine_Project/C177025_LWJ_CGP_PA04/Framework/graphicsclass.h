////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "cameraclass.h"
#include "modelclass.h"

#include "textureshaderclass.h"
#include "bitmapclass.h"
#include "textclass.h"

#include "GameObject.h"
#include "Scene.h"
#include "ResourceManager.h"
#include "Rotator.h"
#include "ModelRenderer.h"

/////////////
// GLOBALS //
/////////////
const bool FULL_SCREEN = false;
const bool VSYNC_ENABLED = true;
const float SCREEN_DEPTH = 3000.0f;
const float SCREEN_NEAR = 0.1f;


////////////////////////////////////////////////////////////////////////////////
// Class name: GraphicsClass
////////////////////////////////////////////////////////////////////////////////
class SceneManager;

class GraphicsClass
{
public:
	GraphicsClass();
	GraphicsClass(const GraphicsClass&);
	~GraphicsClass();

	bool Initialize(int, int, HWND);
	void Shutdown();
	bool Frame(int, int);

private:
	bool Render();

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	XMMATRIX m_BaseViewMatrix;
	

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	BitmapClass* m_InfoBitmap;

	TextClass* m_Text;

	HWND m_hwnd;

	bool m_ShowTitleScene;

	// SceneManager
	SceneManager* m_SceneManager;

	// Scene
	Scene* m_Scene;

	//Resource Manager
	ResourceManager* m_ResourceManager;
};

#endif