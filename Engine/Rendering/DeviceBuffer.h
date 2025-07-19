#ifndef EN_DEVICE_BUFFER_H
#define EN_DEVICE_BUFFER_H

#include <iostream>
#include <vector>

#include "glad/glad.h"

#include "RenderObject.h"
#include "Handle.h"

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
class TDeviceBuffer : public CRenderObject, public CHandle {
protected:
    EBufferMode MMode;
    EBufferType MType;
public:
    inline virtual void Create() override
    {
        MMode = EBufferMode::None;
        MType = EBufferType::None;
        glGenBuffers(1, &MId);
    }

    inline virtual void Bind() override
    {
        glBindBuffer(static_cast<uint32_t>(MType), MId);
    }

    inline virtual void Write(const std::vector<TElementType> &Elements)
    {
        glBufferData(static_cast<uint32_t>(MType), Elements.size() * sizeof(TElementType), Elements.data(), static_cast<uint32_t>(MMode));
    }

    inline virtual void Unbind() override
    {
        glBindBuffer(static_cast<uint32_t>(MType), 0);
    }

    inline virtual void Destroy() override
    {
        glDeleteBuffers(1, &MId);
    }

    inline EBufferMode GetMode() const
    {
        return MMode;
    }

    inline void SetMode(EBufferMode Mode)
    {
        MMode = Mode;
    }

    inline EBufferType GetType() const
    {
        return MType;
    }

    inline void SetType(EBufferType Type)
    {
        MType = Type;
    }

    inline void SetModeAndType(EBufferMode Mode, EBufferType Type)
    {
        MMode = Mode;
        MType = Type;
    }
};

#endif