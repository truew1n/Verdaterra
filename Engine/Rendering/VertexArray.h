#ifndef EN_VERTEX_ARRAY_H
#define EN_VERTEX_ARRAY_H

#include <glad/glad.h>
#include <glm/glm.hpp>
#include <glm/gtc/type_ptr.hpp>

#include "DeviceBuffer.h"


typedef struct SVertex {
	glm::vec3 MPosition;
	glm::vec3 MNormal;
	glm::vec2 MUV;

	SVertex() : MPosition(0.0f), MNormal(0.0f), MUV(0.0f) {}
	SVertex(glm::vec3 NPosition, glm::vec2 NUV) : MPosition(NPosition), MNormal(0.0f), MUV(NUV) {}
	SVertex(glm::vec3 NPosition, glm::vec3 NNormal, glm::vec2 IUV) : MPosition(NPosition), MNormal(NNormal), MUV(IUV) {}
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


template<typename T>
class TVertexArray {
private:
	uint32_t MId;
public:
	TVertexArray()
    {
        glGenVertexArrays(1, &MId);
    }

	void Bind()
	{
		glBindVertexArray(MId);
	}

	void LinkAttribute(uint32_t Layout, uint32_t Count, EVertexComponentType Type, uint8_t Normalized, uint64_t Stride, void *Offset)
    {
        uint8_t ConstNormalized = GL_FALSE * !Normalized + GL_TRUE * Normalized;
        glVertexAttribPointer(Layout, Count, static_cast<uint32_t>(Type), ConstNormalized, static_cast<GLsizei>(Stride), Offset);
        glEnableVertexAttribArray(Layout);
    }

	void Unbind()
    {
        glBindVertexArray(0);
    }

	~TVertexArray()
    {
        glDeleteVertexArrays(1, &MId);
    }
};

#endif