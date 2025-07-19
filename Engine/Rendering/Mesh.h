#ifndef EN_MESH_H
#define EN_MESH_H

#include <vector>

#include "RenderObject.h"
#include "VertexArray.h"


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
class TMesh : public CRenderObject {
private:
    EBufferMode MMode;
    uint32_t MPolygonCount;
    
    TVertexArray<TVertexType> MVertexArray;
    TDeviceBuffer<TVertexType> MVertexBuffer;
    TDeviceBuffer<TIndexType> MElementBuffer;

public:
    inline virtual void Create() override
    {
        MMode = EBufferMode::None;
        MPolygonCount = 0;
        MVertexArray.Create();
        MVertexBuffer.Create();
        MElementBuffer.Create();
    }

    inline virtual void Bind() override
    {
        MVertexArray.Bind();
    }

    void Write(const std::vector<TVertexType> &Vertices, const std::vector<TIndexType> &Indices, const std::vector<SLinkRule> &LinkRules, EBufferMode Mode)
    {
        MVertexBuffer.SetModeAndType(Mode, EBufferType::ArrayBuffer);
        MElementBuffer.SetModeAndType(Mode, EBufferType::ElementArrayBuffer);

        MVertexArray.Bind();
        MVertexBuffer.Bind();
        MElementBuffer.Bind();

        MVertexBuffer.Write(Vertices);
        MElementBuffer.Write(Indices);

        for (const SLinkRule &LinkRule : LinkRules) {
            MVertexArray.LinkAttribute(LinkRule.Layout, LinkRule.Count, LinkRule.Type, LinkRule.Normalized, sizeof(TVertexType), LinkRule.Offset);
        }

        MVertexArray.Unbind();
        MElementBuffer.Unbind();
        MVertexBuffer.Unbind();

        MMode = Mode;
        MPolygonCount = static_cast<uint32_t>(Indices.size());
    }


    inline virtual void Draw()
    {
        glDrawElements(GL_TRIANGLES, MPolygonCount, GL_UNSIGNED_INT, nullptr);
    }

    uint32_t GetPolygonCount() const
    {
        return MPolygonCount;
    }

    inline virtual void Unbind() override
    {
        MVertexArray.Unbind();
    }

    inline virtual void Destroy() override
	{
        MVertexArray.Destroy();
        MVertexBuffer.Destroy();
        MElementBuffer.Destroy();
	}
};

typedef TMesh<SVertex, uint32_t> DMesh;

#endif
