#ifndef EN_VERTEX_BUFFER_H
#define EN_VERTEX_BUFFER_H

#include "glad/glad.h"
#include "glm/glm.hpp"

#include "RenderObject.h"

#include <vector>


typedef struct SVertex {
	glm::vec3 MPosition;
	glm::vec3 MNormal;
	glm::vec2 MUV;

	SVertex() : MPosition(0.0f), MNormal(0.0f), MUV(0.0f) {}
	SVertex(glm::vec3 NPosition, glm::vec2 NUV) : MPosition(NPosition), MNormal(0.0f), MUV(NUV) {}
	SVertex(glm::vec3 NPosition, glm::vec3 NNormal, glm::vec2 IUV) : MPosition(NPosition), MNormal(NNormal), MUV(IUV) {}
} SVertex;


enum class EVertexComponentType : uint16_t {
	None = 0,
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


template<typename TVertexType>
class TVertexBuffer : public CRenderObject {
public:
	TVertexBuffer()
	{
		glGenBuffers(1, &MId);
	}

	virtual void Bind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, MId);
	}

	void Data(std::vector<TVertexType> &Vertices, EBufferUsage Usage)
	{
		glBufferData(GL_ARRAY_BUFFER, Vertices.size() * sizeof(TVertexType), Vertices.data(), static_cast<uint32_t>(Usage));
	}

	virtual void Unbind() override
	{
		glBindBuffer(GL_ARRAY_BUFFER, 0);
	}

	~TVertexBuffer()
	{
		glDeleteBuffers(1, &MId);
	}
};

#endif