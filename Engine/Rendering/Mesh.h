#ifndef EN_MESH_H
#define EN_MESH_H

#include <vector>

#include "RenderObject.h"
#include "Bind.h"
#include "VertexArray.h"


template<typename TVertexType, typename TIndexType>
class TMesh : public CRenderObject, public IBind {
private:
    EBufferMode MMode;
    uint32_t MPolygonCount;
    
    TVertexArray<TVertexType, TIndexType> MVertexArray;
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

        MVertexBuffer.Write(Vertices);
        MElementBuffer.Write(Indices);

        for (const SLinkRule &LinkRule : LinkRules) {
            MVertexArray.LinkAttribute(LinkRule);
        }

        MVertexArray.LinkVertexBuffer(MVertexBuffer);
        MVertexArray.LinkElementBuffer(MElementBuffer);

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
