#ifndef EN_RENDER_OBJECT_H
#define EN_RENDER_OBJECT_H

#include <stdint.h>

class CRenderObject {
protected:
    uint32_t Id;
public:
    CRenderObject() : Id(0) {}
    uint32_t GetId() const;
};

#endif