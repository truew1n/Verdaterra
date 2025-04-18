#ifndef EN_MESH_H
#define EN_MESH_H

#include "Array.h"

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"


class CMesh {
private:
    EBufferUsage MUsage;
    CVertexBuffer<SVertex> MVertexBuffer;
	CElementBuffer<uint32_t> MElementBuffer;	
	CVertexArray<SVertex> MVertexArray;

    uint32_t MPolyCount;

public:
    CMesh() : MUsage(EBufferUsage::None) {}
	CMesh(TArray<SVertex> *Vertices, TArray<uint32_t> *Indices, EBufferUsage Usage)
	{
        MUsage = Usage;
        if (!Vertices) {
            std::cerr << "CMesh :: Vertices == nullptr :: Failed to create mesh!\n";
            return;
        }
        if (!Indices) {
            std::cerr << "CMesh :: Indices == nullptr :: Failed to create mesh!\n";
            return;
        }
        MVertexArray.Bind();

        MVertexBuffer.Bind();
        MElementBuffer.Bind();
        MVertexBuffer.Data(Vertices, Usage);
        MElementBuffer.Data(Indices, Usage);

        MVertexArray.LinkAttribute(&MVertexBuffer, 0, 3, EVertexType::Float32, false, sizeof(SVertex), (void *)0);
        MVertexArray.LinkAttribute(&MVertexBuffer, 1, 3, EVertexType::Float32, false, sizeof(SVertex), (void *)(3 * sizeof(float)));
        MVertexArray.LinkAttribute(&MVertexBuffer, 2, 2, EVertexType::Float32, false, sizeof(SVertex), (void *)(6 * sizeof(float)));

        MVertexArray.Unbind();
        MElementBuffer.Unbind();
        MVertexBuffer.Unbind();

        MPolyCount = Indices->Num();
	}

    void Draw()
    {
        MVertexArray.Bind();
        glDrawElements(GL_TRIANGLES, MPolyCount, GL_UNSIGNED_INT, 0);
        MVertexArray.Unbind();
    }

	~CMesh()
	{

	}
};

#endif
