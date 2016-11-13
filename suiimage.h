#pragma once

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

