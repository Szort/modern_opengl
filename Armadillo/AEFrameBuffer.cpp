#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "AEFrameBuffer.h"

void AEFrameBuffer::CreateFrameBuffer()
{
	glCreateFramebuffers(1, &framebuffer);
	glCreateTextures(GL_TEXTURE_2D, 1, &color_texture);
	glCreateTextures(GL_TEXTURE_2D, 1, &normal_texture);
	glCreateTextures(GL_TEXTURE_2D, 1, &depth_texture);

	glTextureStorage2D(color_texture, 1, GL_RGB8, 1280, 720);
	glTextureParameteri(color_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(color_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(normal_texture, 1, GL_RGB8, 1280, 720);
	glTextureParameteri(normal_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(normal_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(depth_texture, 1, GL_DEPTH_COMPONENT24, 1280, 720);
	glTextureParameteri(depth_texture, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(depth_texture, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, color_texture, 0);
	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT1, normal_texture, 0);
	glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, depth_texture, 0);

	unsigned int DrawAttachemnts[2] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1
	};

	glNamedFramebufferDrawBuffers(framebuffer, 2, DrawAttachemnts);

	uint32_t status = glCheckNamedFramebufferStatus(framebuffer, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not created!" << std::endl;
	}

	glObjectLabel(GL_FRAMEBUFFER, framebuffer, -1, "GBuffer Framebuffer");
	glObjectLabel(GL_TEXTURE, color_texture, -1, "GBuffer Color");
	glObjectLabel(GL_TEXTURE, normal_texture, -1, "GBuffer Normal");
	glObjectLabel(GL_TEXTURE, depth_texture, -1, "GBuffer Depth");
}

void AEFrameBuffer::BindForDraw()
{
	glBindFramebuffer(GL_DRAW_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AEFrameBuffer::BindForRead()
{
	glBindFramebuffer(GL_READ_FRAMEBUFFER, framebuffer);
}

void AEFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}