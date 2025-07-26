#ifndef EN_DEVICE_BUFFER_H
#define EN_DEVICE_BUFFER_H

#include <iostream>
#include <vector>

#include "glad/glad.h"

#include "RenderObject.h"
#include "Handle.h"

enum class EBufferMode : uint32_t {
    StaticDraw = GL_STATIC_DRAW,
    DynamicDraw = GL_DYNAMIC_DRAW,
    StreamDraw = GL_STREAM_DRAW,

    StaticRead = GL_STATIC_READ,
    DynamicRead = GL_DYNAMIC_READ,
    StreamRead = GL_STREAM_READ,

    StaticCopy = GL_STATIC_COPY,
    DynamicCopy = GL_DYNAMIC_COPY,
    StreamCopy = GL_STREAM_COPY,
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
        glCreateBuffers(1, &MId);
    }

    inline virtual void Bind(uint32_t Index)
    {
        glBindBufferBase(static_cast<uint32_t>(MType), Index, MId);
    }

    inline virtual void Write(const std::vector<TElementType> &Elements)
    {
        glNamedBufferData(MId, Elements.size() * sizeof(TElementType), Elements.data(), static_cast<uint32_t>(MMode));
    }

    inline virtual void Read(std::vector<TElementType> &Elements)
    {
        uint32_t BufferSize = 0;
        glGetNamedBufferParameteriv(MId, GL_BUFFER_SIZE, reinterpret_cast<GLint *>(&BufferSize));
        
        uint64_t ElementCount = BufferSize / sizeof(TElementType);
        Elements.reserve(ElementCount);

        glGetNamedBufferSubData(MId, 0, BufferSize, Elements.data());
    }

    inline virtual void Unbind(uint32_t Index)
    {
        glBindBufferBase(static_cast<uint32_t>(MType), Index, 0);
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