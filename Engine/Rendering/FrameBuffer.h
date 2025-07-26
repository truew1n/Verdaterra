#ifndef EN_FRAMEBUFFER_H
#define EN_FRAMEBUFFER_H

#include "RenderObject.h"
#include "Handle.h"
#include "Bind.h"

#include "Texture.h"


enum class EFrameBufferTarget : uint32_t {
	Draw = GL_DRAW_FRAMEBUFFER,
	Read = GL_READ_FRAMEBUFFER,
	Both = GL_FRAMEBUFFER,
	None = 0
};


class CFrameBuffer : public CRenderObject, public CHandle, public IBind {
private:
	EFrameBufferTarget MTarget;
public:
	virtual void Create() override;
	virtual void Bind() override;

	virtual void LinkTexture(CTexture &Texture);

	virtual void Unbind() override;
	virtual void Destroy() override;

	EFrameBufferTarget GetTarget() const;
	void SetTarget(EFrameBufferTarget Target);
};

#endif

