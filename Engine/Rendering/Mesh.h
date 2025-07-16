#ifndef EN_MESH_H
#define EN_MESH_H

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
class TMesh {
private:
    EBufferMode MMode;
    uint32_t MPolygonCount;
    
    TVertexArray<TVertexType> *MVertexArray;
    TDeviceBuffer<TVertexType> *MVertexBuffer;
    TDeviceBuffer<TIndexType> *MElementBuffer;

public:
    TMesh()
    {
        MMode = EBufferMode::None;
        MPolygonCount = 0;
        MVertexArray = nullptr;
        MVertexBuffer = nullptr;
        MElementBuffer = nullptr;
    }

    void Data(const std::vector<TVertexType> &Vertices, const std::vector<TIndexType> &Indices, const std::vector<SLinkRule> &LinkRules, EBufferMode Mode)
    {
        MVertexArray = new TVertexArray<TVertexType>();
        MVertexBuffer = new TDeviceBuffer<TVertexType>();
        MElementBuffer = new TDeviceBuffer<TIndexType>();

        MVertexBuffer->SetModeAndType(Mode, EBufferType::ArrayBuffer);
        MElementBuffer->SetModeAndType(Mode, EBufferType::ElementArrayBuffer);

        MVertexArray->Bind();
        MVertexBuffer->Bind();
        MElementBuffer->Bind();

        MVertexBuffer->Data(Vertices);
        MElementBuffer->Data(Indices);

        for (const SLinkRule &LinkRule : LinkRules) {
            MVertexArray->LinkAttribute(LinkRule.Layout, LinkRule.Count, LinkRule.Type, LinkRule.Normalized, sizeof(TVertexType), LinkRule.Offset);
        }

        MVertexArray->Unbind();
        MElementBuffer->Unbind();
        MVertexBuffer->Unbind();

        MMode = Mode;
        MPolygonCount = static_cast<uint32_t>(Indices.size());
    }

    void Draw()
    {
        MVertexArray->Bind();
        glDrawElements(GL_TRIANGLES, MPolygonCount, GL_UNSIGNED_INT, nullptr);
    }

    uint32_t GetPolygonCount() const
    {
        return MPolygonCount;
    }

	~TMesh()
	{
        if (MVertexArray) delete MVertexArray;
        if (MVertexBuffer) delete MVertexBuffer;
        if (MElementBuffer) delete MElementBuffer;
	}
};

typedef TMesh<SVertex, uint32_t> DMesh;

#endif
