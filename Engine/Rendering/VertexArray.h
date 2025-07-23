#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "RenderObject.h"
#include "Handle.h"
#include "Bind.h"
#include "DeviceBuffer.h"


typedef struct SVertex {
	glm::vec3 MPosition;
	glm::vec3 MNormal;
	glm::vec2 MUV;
} SVertex;


enum class EVertexComponentType : uint32_t {
	Int8 = GL_BYTE,
	UInt8 = GL_UNSIGNED_BYTE,
	Int16 = GL_SHORT,
	UInt16 = GL_UNSIGNED_SHORT,
	Int32 = GL_INT,
	UInt32 = GL_UNSIGNED_INT,
	Float16 = GL_HALF_FLOAT,
	Float32 = GL_FLOAT,
	Float64 = GL_DOUBLE,
	None = 0
};


typedef struct SLinkRule {
	uint32_t Layout;
	uint32_t Count;
	EVertexComponentType Type;
	uint8_t Normalized;
	uint32_t Offset;
} SLinkRule;


template<typename TVertexType, typename TIndexType>
class TVertexArray : public CRenderObject, public CHandle, public IBind {
public:
	inline virtual void Create() override
    {
		glCreateVertexArrays(1, &MId);
	}

	inline virtual void Bind() override
	{
		glBindVertexArray(MId);
	}

	inline void LinkAttribute(const SLinkRule &LinkRule)
    {
		uint8_t ConstNormalized = GL_FALSE * !LinkRule.Normalized + GL_TRUE * LinkRule.Normalized;
		glEnableVertexArrayAttrib(MId, LinkRule.Layout);
		glVertexArrayAttribBinding(MId, LinkRule.Layout, 0);
		glVertexArrayAttribFormat(MId, LinkRule.Layout, LinkRule.Count, static_cast<uint32_t>(LinkRule.Type), ConstNormalized, LinkRule.Offset);
    }

	inline void LinkVertexBuffer(const TDeviceBuffer<TVertexType> &VertexBuffer)
	{
		glVertexArrayVertexBuffer(MId, 0, VertexBuffer.GetId(), 0, sizeof(TVertexType));
	}

	inline void LinkElementBuffer(const TDeviceBuffer<TIndexType> &ElementBuffer)
	{
		glVertexArrayElementBuffer(MId, ElementBuffer.GetId());
	}

	inline virtual void Unbind() override
    {
		glBindVertexArray(0);
    }

	inline virtual void Destroy() override
    {
		glDeleteVertexArrays(1, &MId);
    }
};

#endif