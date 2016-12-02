#pragma once

#include "sui.h"

namespace sui{


class SUISurfaceUtility
{
public:
    SUISurfaceUtility();
    static SUISurface rotate(SUISurface &arg, const int angle);
    static SUISurface mirror(SUISurface &arg);

};

}

