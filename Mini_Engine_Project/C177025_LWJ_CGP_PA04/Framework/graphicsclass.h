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
#include "Rotator.h"
#include "ModelRenderer.h"
#include "Scene.h"
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
	bool Render(float);

private:
	D3DClass* m_D3D;
	CameraClass* m_Camera;
	XMMATRIX m_BaseViewMatrix;

	// Models
	ModelClass* m_WaterPlane;

	ModelClass* m_FloatingIsland;
	ModelClass* m_FloatingAlter;
	ModelClass* m_GuardianLion;
	ModelClass* m_CrystalPrism;
	ModelClass* m_Cloud;
	ModelClass* m_SakuraTree;
	ModelClass* m_CrystalPrism2;
	ModelClass* m_CircleStone;
	ModelClass* m_GoldenSpire;
	ModelClass* m_Lantern;

	ModelClass* m_Billboard;

	TextureShaderClass* m_TextureShader;
	BitmapClass* m_Bitmap;
	BitmapClass* m_InfoBitmap;

	TextClass* m_Text;

	HWND m_hwnd;

	bool m_ShowTitleScene;

	// 각 오브젝트 필요한 멤버변수 추가
	GameObject* m_GuardianLionObject;
	GameObject* m_SakuraTreeObject;

	// Scene
	Scene* m_Scene;
};

#endif