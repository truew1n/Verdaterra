#ifndef EN_MESH_H
#define EN_MESH_H

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <vector>

class CMesh {
private:
    EBufferUsage MUsage;
    TVertexArray<SVertex> MVertexArray;
    TVertexBuffer<SVertex> MVertexBuffer;
    TElementBuffer<uint32_t> MElementBuffer;

    uint32_t MPolygonCount;

public:
    CMesh(std::vector<SVertex> &NVertices, std::vector<uint32_t> &NIndices, EBufferUsage NUsage)
	{
        MUsage = EBufferUsage::None;
        MPolygonCount = 0;
        
        MVertexArray.Bind();
        MVertexBuffer.Bind();
        MElementBuffer.Bind();

        MVertexBuffer.Data(NVertices, NUsage);
        MElementBuffer.Data(NIndices, NUsage);

        MVertexArray.LinkAttribute(MVertexBuffer, 0, 3, EVertexType::Float32, false, sizeof(SVertex), (void *)0);
        MVertexArray.LinkAttribute(MVertexBuffer, 1, 3, EVertexType::Float32, false, sizeof(SVertex), (void *)(3 * sizeof(float)));
        MVertexArray.LinkAttribute(MVertexBuffer, 2, 2, EVertexType::Float32, false, sizeof(SVertex), (void *)(6 * sizeof(float)));

        MVertexArray.Unbind();
        MElementBuffer.Unbind();
        MVertexBuffer.Unbind();
        

        MUsage = NUsage;
        MPolygonCount = (uint32_t) NIndices.size();
	}

    void Draw()
    {
        MVertexArray.Bind();
        glDrawElements(GL_TRIANGLES, MPolygonCount, GL_UNSIGNED_INT, nullptr);
        MVertexArray.Unbind();
    }

    uint32_t GetPolygonCount() const
    {
        return MPolygonCount;
    }

	~CMesh()
	{

	}
};

#endif
