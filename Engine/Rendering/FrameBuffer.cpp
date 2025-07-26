#include "FrameBuffer.h"


void CFrameBuffer::Create()
{
	glCreateFramebuffers(1, &MId);
}

void CFrameBuffer::Bind()
{
	glBindFramebuffer(static_cast<uint32_t>(MTarget), MId);
}

void CFrameBuffer::LinkTexture(CTexture &Texture)
{
	glNamedFramebufferTexture(MId, static_cast<uint32_t>(Texture.GetFormat()), Texture.GetId(), 0);
}

void CFrameBuffer::Unbind()
{
	glBindFramebuffer(static_cast<uint32_t>(MTarget), 0);
}

void CFrameBuffer::Destroy()
{
	glDeleteFramebuffers(1, &MId);
}

EFrameBufferTarget CFrameBuffer::GetTarget() const
{
	return MTarget;
}

void CFrameBuffer::SetTarget(EFrameBufferTarget Target)
{
	MTarget = Target;
}

//GLuint ShadowMapFBO;
//GLuint ShadowMap;
//glCreateTextures(GL_TEXTURE_2D, 1, &ShadowMap);
//glTextureStorage2D(ShadowMap, 1, GL_DEPTH_COMPONENT32F, 2048, 2048);
//
//glTextureParameteri(ShadowMap, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
//glTextureParameteri(ShadowMap, GL_TEXTURE_MAG_FILTER, GL_NEAREST);
//glTextureParameteri(ShadowMap, GL_TEXTURE_WRAP_S, GL_CLAMP_TO_BORDER);
//glTextureParameteri(ShadowMap, GL_TEXTURE_WRAP_T, GL_CLAMP_TO_BORDER);
//
//float ClampColor[] = { 1.0f, 1.0f, 1.0f, 1.0f };
//glTextureParameterfv(ShadowMap, GL_TEXTURE_BORDER_COLOR, ClampColor);
//
//
//glNamedFramebufferTexture(ShadowMapFBO, GL_DEPTH_ATTACHMENT, ShadowMap, 0);
//
//
//glNamedFramebufferDrawBuffer(ShadowMapFBO, GL_NONE);
//glNamedFramebufferReadBuffer(ShadowMapFBO, GL_NONE);
//
//
//if (glCheckNamedFramebufferStatus(ShadowMapFBO, GL_FRAMEBUFFER) != GL_FRAMEBUFFER_COMPLETE) {
//   
//}