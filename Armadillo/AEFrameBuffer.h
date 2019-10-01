#pragma once

class AEFrameBuffer
{
	unsigned int framebuffer;
	unsigned int color_texture;
	unsigned int normal_texture;
	unsigned int depth_texture;

public:

	void CreateFrameBuffer();
	void BindForDraw();
	void BindForRead();
	void Unbind();
};