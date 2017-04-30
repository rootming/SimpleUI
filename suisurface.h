#pragma once

#include <cstring>
#include "suiobject.h"
#include "suitype.h"

namespace sui {

class SUISurface: public SUIObject
{
public:
    SUISurface(SUIObject *parent = nullptr);
    SUISurface(const int64_t w, const int64_t h, SUIObject *parent = nullptr);


    SUISurface(const SUISurface& from): SUIObject(from.getParent())
    {
        this->x = from.x;
        this->y = from.y;
        this->resize(this->getData().getWidth(), this->getData().getHeight());
        memcpy(this->getData().buffer, from.getData().buffer, from.getData().bytes());
    }

    virtual void resize(const pos_t w, const pos_t h)
    {
        data.setWidth(w);
        data.setHeight(h);
        data.setDepth(DEPTH32);
        data.setScanLineSize();
        if(data.buffer != nullptr)
            delete []data.buffer;
        data.buffer = new uint8_t[data.bytes()];
        memset(data.buffer, 0, data.bytes());
    }

    virtual void update(void)
    {

    }

    const SUIData& getData() const { return data; }


protected:
    SUIData data;

private:
    pos_t x, y;
    void init();

};

}

