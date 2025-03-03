#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "VertexBuffer.h"

template<typename T>
class CVertexArray : public CRenderObject {
public:
	CVertexArray()
    {
        glGenVertexArrays(1, &Id);
    }

	void LinkAttribute(CVertexBuffer<T> *VertexBuffer, uint32_t Layout, uint32_t ComponentCount, uint32_t Type, uint64_t Stride, void *Offset)
    {
        if (!VertexBuffer) {
            std::cerr << "CVertexArray :: VertexBuffer == nullptr :: Failed to link attribute!\n";
            return;
        }
        VertexBuffer->Bind();
        glVertexAttribPointer(Layout, ComponentCount, Type, GL_FALSE, (GLsizei) Stride, Offset);
        glEnableVertexAttribArray(Layout);
    }

	virtual void Bind() override
    {
        glBindVertexArray(Id);
    }

	virtual void Unbind() override
    {
        glBindVertexArray(0);
    }

	~CVertexArray()
    {
        glDeleteVertexArrays(1, &Id);
    }
};

#endif