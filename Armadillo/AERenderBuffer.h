#pragma once

class AERenderBuffer
{
	unsigned int renderbufferID;
	unsigned int texture_colorID;
	unsigned int texture_depthID;

public:

	void CreateRenderBuffer();
	void BindRenderBuffer();
	void UnbindRenderBuffer();
};