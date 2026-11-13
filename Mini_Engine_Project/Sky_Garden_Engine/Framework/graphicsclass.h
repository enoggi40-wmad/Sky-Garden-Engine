////////////////////////////////////////////////////////////////////////////////
// Filename: graphicsclass.h
////////////////////////////////////////////////////////////////////////////////
#ifndef _GRAPHICSCLASS_H_
#define _GRAPHICSCLASS_H_


///////////////////////
// MY CLASS INCLUDES //
///////////////////////
#include "d3dclass.h"
#include "ShaderManager.h"
#include "ResourceManager.h"
#include "PerformanceMonitor.h"
#include "textclass.h"


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

    PerformanceMonitor m_PerformanceMonitor;

    TextClass* m_Text = nullptr;

    bool m_ShowPerformanceMonitor = true;
    bool m_PreviousF4Pressed = false;

    float m_PerformanceTextTimer = 0.0f;

private:
    // Direct3D
    D3DClass* m_D3D;

    // Texture Shader
    ShaderManager* m_ShaderManager;

    // Scene Manager
    SceneManager* m_SceneManager;

    // Resource Manager
    ResourceManager* m_ResourceManager;
};

#endif