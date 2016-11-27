#include "suisurface.h"

namespace sui {

SUISurface::SUISurface(SUISurface *parent): SUIObject(parent)
{
    init();
}

SUISurface::SUISurface(const int64_t w, const int64_t h, SUISurface *parent): SUIObject(parent)
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
