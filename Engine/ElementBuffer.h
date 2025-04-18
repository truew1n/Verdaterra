#ifndef EN_ELEMENT_BUFFER_H
#define EN_ELEMENT_BUFFER_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "Array.h"

template<typename T>
class CElementBuffer : public CRenderObject {
public:
    CElementBuffer()
    {
        glGenBuffers(1, &Id);
    }

    virtual void Bind() override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
    }

    void Data(TArray<T> *Indices, EBufferUsage Usage)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices->Num() * sizeof(T), Indices->begin(), (uint32_t) Usage);
    }

    virtual void Unbind() override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~CElementBuffer()
    {
        glDeleteBuffers(1, &Id);
    }
};

#endif