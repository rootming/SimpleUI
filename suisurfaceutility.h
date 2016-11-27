#pragma once

#include "sui.h"

namespace sui{


class SUISurfaceUtility
{
public:
    SUISurfaceUtility();
    SUISurface rotate(SUISurface &arg, const int angle);
    SUISurface mirror(SUISurface &arg);
};

}

