#pragma once

#include "AEViewport.h"

enum AEGBufferTexture
{
	eAE_GBuffer_Albedo,
	eAE_GBuffer_Normal,
	eAE_GBuffer_Depth,
	eAE_GBuffer_Count
};

class AEFrameBuffer
{
	uint32_t	framebuffer;
	uint32_t	textures[eAE_GBuffer_Count];

public:

	void CreateFrameBuffer(AEViewport& viewport);
	void BindForDraw();
	void BindForRead();
	void Unbind();

	uint32_t* GetTexture() { return textures; };
	uint32_t GetTexture(AEGBufferTexture id) { return textures[id]; };
};