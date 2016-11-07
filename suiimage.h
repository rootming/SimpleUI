#ifndef SUIIMAGE_H
#define SUIIMAGE_H

#include "suitype.h"
#include "suisurface.h"

using namespace sui;

namespace sui {

class SUIImage
{
public:
    SUIImage(SUISurface &surface);
    void save(const char *name) const;

private:
    SUIData data;
};

}

#endif // SUIIMAGE_H
