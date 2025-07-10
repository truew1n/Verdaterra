#ifndef EN_ELEMENT_BUFFER_H
#define EN_ELEMENT_BUFFER_H

#include <glad/glad.h>

#include "RenderObject.h"

#include <vector>


template<typename TIndexType>
class TElementBuffer : public CRenderObject {
public:
    TElementBuffer()
    {
        glGenBuffers(1, &MId);
    }

    virtual void Bind() override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, MId);
    }

    void Data(std::vector<TIndexType> &Indices, EBufferUsage Usage)
    {
        glBufferData(GL_ELEMENT_ARRAY_BUFFER, Indices.size() * sizeof(TIndexType), Indices.data(), static_cast<uint32_t>(Usage));
    }

    virtual void Unbind() override
    {
        glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, 0);
    }

    ~TElementBuffer()
    {
        glDeleteBuffers(1, &MId);
    }
};

#endif