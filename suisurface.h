#ifndef SUISURFACE_H
#define SUISURFACE_H
#include <cstring>
#include "suiobject.h"
#include "basetype.h"

class SUISurface: public SUIObject
{
public:
    SUISurface(SUISurface *parent = nullptr);
    virtual void resize(const int32_t w, const int32_t h)
    {
        data.setWidth(w);
        data.setHeight(h);
        if(data.buffer != nullptr)
            delete []data.buffer;
        data.buffer = new int32_t[data.bytes() / sizeof(int32_t)];
        memset(data.buffer, 0, data.bytes());
    }

    virtual void update(void)
    {

    }

protected:
    SUIData data;

};

#endif // SUISURFACE_H
