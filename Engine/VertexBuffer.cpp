#include "VertexBuffer.h"

CVertexBuffer::CVertexBuffer(TArray<SVertex> *Vertices)
{
    glGenBuffers(1, &Id);
    glBindBuffer(GL_ARRAY_BUFFER, Id);
    glBufferData(GL_ARRAY_BUFFER, Vertices->Num() * sizeof(SVertex), Vertices->begin(), GL_STATIC_DRAW);
}

void CVertexBuffer::Bind()
{
    glBindBuffer(GL_ARRAY_BUFFER, Id);
}

void CVertexBuffer::Unbind()
{
    glBindBuffer(GL_ARRAY_BUFFER, 0);
}

void CVertexBuffer::Delete()
{
    glDeleteBuffers(1, &Id);
}