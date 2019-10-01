#include "GL/glew.h"
#include <GLFW/glfw3.h>

#include "AERenderBuffer.h"

void AERenderBuffer::CreateRenderBuffer()
{
	glCreateBuffers(1, &renderbufferID);
	glNamedRenderbufferStorage(renderbufferID, GL_RGB, 1280, 720);

	glCreateTextures(GL_TEXTURE_2D, 1, &texture_colorID);
	glTextureStorage2D(GL_TEXTURE_2D, 1, GL_RGB8, 1280, 720);
	glTextureParameteri(texture_colorID, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture_colorID, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_EDGE);
	glTextureParameteri(texture_colorID, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture_colorID, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glObjectLabel(GL_RENDERBUFFER, renderbufferID, -1, "Select Renderbuffer");
	glObjectLabel(GL_TEXTURE, texture_colorID, -1, "Select Texture");
}

void AERenderBuffer::BindRenderBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, renderbufferID);
}

void AERenderBuffer::UnbindRenderBuffer()
{
	glBindRenderbuffer(GL_RENDERBUFFER, 0);
}