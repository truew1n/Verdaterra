#ifndef EN_ELEMENT_BUFFER_H
#define EN_ELEMENT_BUFFER_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "Array.h"

class CElementBuffer : public CRenderObject {
public:
    CElementBuffer(TArray<uint32_t> *Indices);

    void Bind();
    void Unbind();
    void Delete();
};

#endif

