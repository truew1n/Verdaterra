#ifndef EN_VERTEX_BUFFER_H
#define EN_VERTEX_BUFFER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "RenderObject.h"

#include "Array.h"

typedef struct SVertex {
	glm::vec3 Position;
	glm::vec3 Normal;
	glm::vec2 UV;

	SVertex() : Position(0.0f), Normal(0.0f), UV(0.0f) {}
	SVertex(glm::vec3 IPosition, glm::vec2 IUV) : Position(IPosition), Normal(0.0f), UV(IUV) {}
	SVertex(glm::vec3 IPosition, glm::vec3 INormal, glm::vec2 IUV) : Position(IPosition), Normal(INormal), UV(IUV) {}
} SVertex;

enum class EVertexType : uint16_t {
	Int8 = GL_BYTE,
	UInt8 = GL_UNSIGNED_BYTE,
	Int16 = GL_SHORT,
	UInt16 = GL_UNSIGNED_SHORT,
	Int32 = GL_INT,
	UInt32 = GL_UNSIGNED_INT,
	Float16 = GL_HALF_FLOAT,
	Float32 = GL_FLOAT,
	Float64 = GL_DOUBLE
};

template<typename T>
class CVertexBuffer : public CRenderObject {
public:
	CVertexBuffer(TArray<T> *Vertices)
	{
		glGenBuffers(1, &Id);
		glBindBuffer(GL_ARRAY_BUFFER, Id);
		glBufferData(GL_ARRAY_BUFFER, Vertices->Num() * sizeof(T), Vertices->begin(), GL_STATIC_DRAW);
	}

	virtual void Bind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, Id);
	}

	virtual void Unbind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~CVertexBuffer()
	{
		glDeleteBuffers(1, &Id);
	}
};

#endif