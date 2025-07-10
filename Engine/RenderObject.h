#ifndef EN_RENDER_OBJECT_H
#define EN_RENDER_OBJECT_H

#include "glad/glad.h"

#include <stdint.h>


enum class EBufferUsage : uint32_t {
    None = 0,

    StreamDraw = GL_STREAM_DRAW,
    StreamRead = GL_STREAM_READ,
    StreamCopy = GL_STREAM_COPY,

    StaticDraw = GL_STATIC_DRAW,
    StaticRead = GL_STATIC_READ,
    StaticCopy = GL_STATIC_COPY,

    DynamicDraw = GL_DYNAMIC_DRAW,
    DynamicRead = GL_DYNAMIC_READ,
    DynamicCopy = GL_DYNAMIC_COPY
};


class CRenderObject {
protected:
    uint32_t MId;
public:
    CRenderObject() : MId(0) {}
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    uint32_t GetId() const;
};

#endif