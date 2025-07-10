#ifndef EN_MESH_H
#define EN_MESH_H

#include "ElementBuffer.h"
#include "VertexBuffer.h"
#include "VertexArray.h"

#include <vector>


typedef struct SLinkRule {
    uint32_t Layout;
    uint32_t Count;
    EVertexComponentType Type;
    uint8_t Normalized;
    void *Offset;

    SLinkRule() :
        Layout(0),
        Count(0),
        Type(EVertexComponentType::None),
        Normalized(false),
        Offset(nullptr)
    { }

    SLinkRule(
        uint32_t NLayout,
        uint32_t NCount,
        EVertexComponentType NType,
        uint8_t NNormalized,
        void *NOffset
    ) :
        Layout(NLayout),
        Count(NCount),
        Type(NType),
        Normalized(NNormalized),
        Offset(NOffset)
    { }
} SLinkRule;


template<typename TVertexType, typename TIndexType>
class CMesh {
private:
    EBufferUsage MUsage;
    uint32_t MPolygonCount;
    
    TVertexArray<TVertexType> MVertexArray;
    TVertexBuffer<TVertexType> MVertexBuffer;
    TElementBuffer<TIndexType> MElementBuffer;

public:
    CMesh() : MUsage(EBufferUsage::None), MPolygonCount(0) {}

    void Data(std::vector<TVertexType> &Vertices, std::vector<TIndexType> &Indices, std::vector<SLinkRule> &LinkRules, EBufferUsage Usage)
    {
        MVertexArray.Bind();
        MVertexBuffer.Bind();
        MElementBuffer.Bind();

        MVertexBuffer.Data(Vertices, Usage);
        MElementBuffer.Data(Indices, Usage);

        for (const SLinkRule &LinkRule : LinkRules) {
            MVertexArray.LinkAttribute(LinkRule.Layout, LinkRule.Count, LinkRule.Type, LinkRule.Normalized, sizeof(TVertexType), LinkRule.Offset);
        }

        MVertexArray.Unbind();
        MElementBuffer.Unbind();
        MVertexBuffer.Unbind();

        MUsage = Usage;
        MPolygonCount = static_cast<uint32_t>(Indices.size());
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
