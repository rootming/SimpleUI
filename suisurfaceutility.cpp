#include "suisurfaceutility.h"


using namespace sui;


namespace sui{

SUISurfaceUtility::SUISurfaceUtility()
{

}

SUISurface SUISurfaceUtility::rotate(SUISurface &arg, const int angle)
{


}

SUISurface SUISurfaceUtility::mirror(SUISurface &arg)
{
    SUISurface tmp(arg.getData().getWidth(), arg.getData().getHeight());
    const pos_t pos = arg.getData().getScanlineSize();
    pos_t p = 0;
    for(pos_t y = 0; y < arg.getData().getHeight(); y++){
        for(pos_t x = 0; y < arg.getData().getWidth(); x++){
            memcpy(&tmp.getData().buffer[p], &arg.getData().buffer[pos + pos * y - x * 3 - 3], 3);
            p += 3;
        }
    }
    return tmp;


}

}
