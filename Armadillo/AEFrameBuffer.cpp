#include <iostream>
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include "AEFrameBuffer.h"

void AEFrameBuffer::CreateFrameBuffer(AEViewport& viewport)
{
	glCreateFramebuffers(1, &framebuffer);
	glCreateTextures(GL_TEXTURE_2D, eAE_GBuffer_Count, &textures[0]);
	glCreateTextures(GL_TEXTURE_2D, 1, &texture_picking);

	glTextureStorage2D(textures[eAE_GBuffer_Albedo], 1, GL_RGB8, viewport.GetSize()->x, viewport.GetSize()->y);
	glTextureParameteri(textures[eAE_GBuffer_Albedo], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textures[eAE_GBuffer_Albedo], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(textures[eAE_GBuffer_Normal], 1, GL_RGB8, viewport.GetSize()->x, viewport.GetSize()->y);
	glTextureParameteri(textures[eAE_GBuffer_Normal], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textures[eAE_GBuffer_Normal], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(textures[eAE_GBuffer_Depth], 1, GL_DEPTH_COMPONENT24, viewport.GetSize()->x, viewport.GetSize()->y);
	glTextureParameteri(textures[eAE_GBuffer_Depth], GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(textures[eAE_GBuffer_Depth], GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glTextureStorage2D(texture_picking, 1, GL_RGB8, viewport.GetSize()->x, viewport.GetSize()->y);
	glTextureParameteri(texture_picking, GL_TEXTURE_MIN_FILTER, GL_LINEAR);
	glTextureParameteri(texture_picking, GL_TEXTURE_MAG_FILTER, GL_LINEAR);

	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT0, textures[eAE_GBuffer_Albedo], 0);
	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT1, textures[eAE_GBuffer_Normal], 0);
	glNamedFramebufferTexture(framebuffer, GL_COLOR_ATTACHMENT2, texture_picking, 0);
	glNamedFramebufferTexture(framebuffer, GL_DEPTH_ATTACHMENT, textures[eAE_GBuffer_Depth], 0);

	uint32_t DrawAttachemnts[3] = {
		GL_COLOR_ATTACHMENT0,
		GL_COLOR_ATTACHMENT1,
		GL_COLOR_ATTACHMENT2
	};

	glNamedFramebufferDrawBuffers(framebuffer, 3, DrawAttachemnts);

	uint32_t status = glCheckNamedFramebufferStatus(framebuffer, GL_FRAMEBUFFER);
	if (status != GL_FRAMEBUFFER_COMPLETE) {
		std::cout << "Framebuffer not created!" << std::endl;
	}

	glObjectLabel(GL_FRAMEBUFFER, framebuffer, -1, "GBuffer Framebuffer");
	glObjectLabel(GL_TEXTURE, textures[eAE_GBuffer_Albedo], -1, "GBuffer Color");
	glObjectLabel(GL_TEXTURE, textures[eAE_GBuffer_Normal], -1, "GBuffer Normal");
	glObjectLabel(GL_TEXTURE, textures[eAE_GBuffer_Depth], -1, "GBuffer Depth");
	glObjectLabel(GL_TEXTURE, texture_picking, -1, "GBuffer Selection");
}

void AEFrameBuffer::BindForDraw()
{
	glNamedFramebufferDrawBuffers(framebuffer, 2, GBufferDrawAttachemnts);
}

void AEFrameBuffer::BindForPicking()
{
	glNamedFramebufferDrawBuffers(framebuffer, 1, PickingDrawAttachemnts);
}

void AEFrameBuffer::Bind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, framebuffer);
	glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);
}

void AEFrameBuffer::Unbind()
{
	glBindFramebuffer(GL_FRAMEBUFFER, 0);
}