#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "VertexBuffer.h"


template<typename T>
class TVertexArray : public CRenderObject {
public:
	TVertexArray()
    {
        glGenVertexArrays(1, &MId);
    }

	void LinkAttribute(uint32_t Layout, uint32_t Count, EVertexComponentType Type, uint8_t Normalized, uint64_t Stride, void *Offset)
    {
        uint8_t ConstNormalized = GL_FALSE * !Normalized + GL_TRUE * Normalized;
        glVertexAttribPointer(Layout, Count, (uint32_t) Type, ConstNormalized, (GLsizei) Stride, Offset);
        glEnableVertexAttribArray(Layout);
    }

	virtual void Bind() override
    {
        glBindVertexArray(MId);
    }

	virtual void Unbind() override
    {
        glBindVertexArray(0);
    }

	~TVertexArray()
    {
        glDeleteVertexArrays(1, &MId);
    }
};

#endif