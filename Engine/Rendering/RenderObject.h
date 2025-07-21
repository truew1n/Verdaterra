#ifndef EN_RENDER_OBJECT_H
#define EN_RENDER_OBJECT_H


class CRenderObject {
public:
    virtual void Create() = 0;
    virtual void Bind() = 0;
    virtual void Unbind() = 0;
    virtual void Destroy() = 0;
};

#endif

