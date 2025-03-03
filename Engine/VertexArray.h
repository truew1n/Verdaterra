#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "VertexBuffer.h"

class CVertexArray : public CRenderObject {
public:
	CVertexArray();

	void LinkAttribute(CVertexBuffer *VertexBuffer, uint32_t Layout, uint32_t ComponentCount, uint32_t Type, ptrdiff_t Stride, void *Offset);
	virtual void Bind() override;
	virtual void Unbind() override;

	~CVertexArray();
};

#endif