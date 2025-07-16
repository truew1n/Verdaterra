#ifndef EN_RENDER_OBJECT_H
#define EN_RENDER_OBJECT_H

#include "glad/glad.h"
#include <iostream>

#include <stdint.h>

#include <vector>

enum class EBufferMode : uint32_t {
    StreamDraw = GL_STREAM_DRAW,
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,

    StreamRead = GL_STREAM_READ,
    StaticRead = GL_STATIC_READ,
    DynamicRead = GL_DYNAMIC_READ,

    StreamCopy = GL_STREAM_COPY,
    StaticCopy = GL_STATIC_COPY,
    DynamicCopy = GL_DYNAMIC_COPY,
    None = 0
};

enum class EBufferType : uint32_t {
    ArrayBuffer = GL_ARRAY_BUFFER,
    AtomicCounterBuffer = GL_ATOMIC_COUNTER_BUFFER,
    CopyReadBuffer = GL_COPY_READ_BUFFER,
    CopyWriteBuffer = GL_COPY_WRITE_BUFFER,
    DispatchIndirectBuffer = GL_DISPATCH_INDIRECT_BUFFER,
    DrawIndirectBuffer = GL_DRAW_INDIRECT_BUFFER,
    ElementArrayBuffer = GL_ELEMENT_ARRAY_BUFFER,
    PixelPackBuffer = GL_PIXEL_PACK_BUFFER,
    PixelUnpackBuffer = GL_PIXEL_UNPACK_BUFFER,
    QueryBuffer = GL_QUERY_BUFFER,
    ShaderStorageBuffer = GL_SHADER_STORAGE_BUFFER,
    TextureBuffer = GL_TEXTURE_BUFFER,
    TransformFeedbackBuffer = GL_TRANSFORM_FEEDBACK_BUFFER,
    UniformBuffer = GL_UNIFORM_BUFFER,
    None = 0
};

template<typename TElementType>
class TDeviceBuffer {
protected:
    uint32_t MId;
    EBufferMode MMode;
    EBufferType MType;
public:
    TDeviceBuffer() 
    {
        MMode = EBufferMode::None;
        MType = EBufferType::None;
        glGenBuffers(1, &MId);
    }

    virtual void Bind()
    {
        glBindBuffer(static_cast<uint32_t>(MType), MId);
    }

    virtual void Data(const std::vector<TElementType> &Elements)
    {
        glBufferData(static_cast<uint32_t>(MType), Elements.size() * sizeof(TElementType), Elements.data(), static_cast<uint32_t>(MMode));
    }

    virtual void Unbind()
    {
        glBindBuffer(static_cast<uint32_t>(MType), 0);
    }

    ~TDeviceBuffer()
    {
        glDeleteBuffers(1, &MId);
    }

    uint32_t GetId() const
    {
        return MId;
    }

    uint32_t SetId(uint32_t Id)
    {
        MId = Id;
    }

    EBufferMode GetMode() const
    {
        return MMode;
    }

    void SetMode(EBufferMode Mode)
    {
        MMode = Mode;
    }

    EBufferType GetType() const
    {
        return MType;
    }

    void SetType(EBufferType Type)
    {
        MType = Type;
    }

    void SetModeAndType(EBufferMode Mode, EBufferType Type)
    {
        MMode = Mode;
        MType = Type;
    }
};

#endif