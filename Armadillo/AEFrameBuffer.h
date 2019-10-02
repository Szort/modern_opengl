#pragma once

#include "AEViewport.h"

class AEFrameBuffer
{
	uint32_t framebuffer;
	uint32_t color_texture;
	uint32_t normal_texture;
	uint32_t depth_texture;

public:

	void CreateFrameBuffer(AEViewport& viewport);
	void BindForDraw();
	void BindForRead();
	void Unbind();

	uint32_t GetColor() { return color_texture; };
	uint32_t GetNormal() { return normal_texture; };
};