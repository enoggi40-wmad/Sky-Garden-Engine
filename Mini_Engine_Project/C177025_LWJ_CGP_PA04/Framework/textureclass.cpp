////////////////////////////////////////////////////////////////////////////////
// Filename: textureclass.cpp
////////////////////////////////////////////////////////////////////////////////
#include "textureclass.h"
#include "DDSTextureLoader.h"
#include <corecrt_wstdio.h>

using namespace DirectX;

TextureClass::TextureClass()
{
	m_texture = 0;
}


TextureClass::TextureClass(const TextureClass& other)
{
}


TextureClass::~TextureClass()
{
}


bool TextureClass::Initialize(
    ID3D11Device* device,
    const WCHAR* filename)
{
    HRESULT result;

    // Load texture data from a file by using DDS texture loader.
    result = CreateDDSTextureFromFile(
        device,
        filename,
        nullptr,
        &m_texture
    );

    if (FAILED(result))
    {
        wchar_t message[256];

        swprintf_s(
            message,
            L"CreateDDSTextureFromFile Failed\nHRESULT: 0x%08X",
            result
        );

        MessageBox(
            NULL,
            message,
            L"Texture Error",
            MB_OK
        );

        return false;
    }

    return true;
}


void TextureClass::Shutdown()
{
	// Release the texture resource.
	if(m_texture)
	{
		m_texture->Release();
		m_texture = 0;
	}

	return;
}


ID3D11ShaderResourceView* TextureClass::GetTexture()
{
	return m_texture;
}