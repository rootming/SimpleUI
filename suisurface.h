#ifndef SUISURFACE_H
#define SUISURFACE_H
#include <cstring>
#include "suiobject.h"
#include "suitype.h"

namespace sui {

class SUISurface: public SUIObject
{
public:
    SUISurface(SUISurface *parent = nullptr);
    SUISurface(const int64_t w, const int64_t h, SUISurface *parent = nullptr);
    virtual void resize(const int64_t w, const int64_t h)
    {
        data.setWidth(w);
        data.setHeight(h);
        if(data.buffer != nullptr)
            delete []data.buffer;
        data.buffer = new uint8_t[data.bytes() / sizeof(uint8_t)];
        memset(data.buffer, 0, data.bytes());
    }

    virtual void update(void)
    {

    }

    const SUIData& getData() const { return data; }

protected:
    SUIData data;

private:
    int64_t x, y;
    void init();

};

}

#endif // SUISURFACE_H
