#ifndef EN_VERTEX_LAYOUT_H
#define EN_VERTEX_LAYOUT_H

#include <stdint.h>

#include "glad/glad.h"

#include "Array.h"

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

typedef struct SVertexEntry {
	uint32_t Layout;
	uint32_t Count;
	EVertexType Type;
	uint32_t Stride;

	SVertexEntry(uint32_t ILayout, uint32_t ICount, EVertexType IType, uint32_t IStride) : Layout(ILayout), Count(ICount), Type(IType), Stride(IStride) {}
} SVertexEntry;

typedef TArray<SVertexEntry> DVertexLayout;

inline uint64_t GetSizeOfVertexType(EVertexType type) {
	switch (type) {
	case EVertexType::Int8:
	case EVertexType::UInt8:   return 1;
	case EVertexType::Int16:
	case EVertexType::UInt16:  return 2;
	case EVertexType::Int32:
	case EVertexType::UInt32:
	case EVertexType::Float32: return 4;
	case EVertexType::Float64: return 8;
	case EVertexType::Float16: return 2;
	default: return 0;
	}
}

#endif

