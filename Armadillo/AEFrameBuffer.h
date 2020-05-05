#pragma once

#include "AEViewport.h"

enum AEGBufferTexture
{
	eAE_GBuffer_WorldPosition,
	eAE_GBuffer_Albedo,
	eAE_GBuffer_Normal,
	eAE_GBuffer_Depth,
	eAE_GBuffer_Count
};

class AEFrameBuffer
{
	uint32_t	framebuffer;
	uint32_t	textures[eAE_GBuffer_Count];

	uint32_t GBufferDrawAttachemnts[3] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
	};

public:

	void CreateFrameBuffer(AEViewport* viewport);
	void BindForDraw();
	void Bind();
	void Unbind();

	uint32_t* GetTexture() { return textures; };
	uint32_t GetTexture(AEGBufferTexture id) { return textures[id]; };
};