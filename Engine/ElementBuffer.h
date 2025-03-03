#ifndef EN_ELEMENT_BUFFER_H
#define EN_ELEMENT_BUFFER_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "Array.h"

template<typename T>
class CElementBuffer : public CRenderObject {
public:
    CElementBuffer(TArray<T> *Indices)
    {
        glGenBuffers(1, &Id);
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices->Num() * sizeof(T), Indices->begin(), GL_STATIC_DRAW);
    }

    virtual void Bind() override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, Id);
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