#ifndef EN_ELEMENT_BUFFER_H
#define EN_ELEMENT_BUFFER_H

#include <glad/glad.h>

#include "RenderObject.h"

#include "Array.h"

class CElementBuffer : public CRenderObject {
public:
    CElementBuffer(TArray<uint32_t> *Indices);

    virtual void Bind() override;
    virtual void Unbind() override;

    ~CElementBuffer();
};

#endif

