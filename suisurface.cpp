#include "suisurface.h"

namespace sui {

SUISurface::SUISurface(SUIObject *parent): SUIObject(parent)
{
    init();
}

SUISurface::SUISurface(const int64_t w, const int64_t h, SUIObject *parent): SUIObject(parent)
{
    init();
    resize(w, h);
}

void SUISurface::init()
{
    x = 0;
    y = 0;
}



}
