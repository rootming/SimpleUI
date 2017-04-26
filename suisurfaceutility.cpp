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
    const pos_t pos = arg.getData().getScanLineSize();
    const pos_t height = arg.getData().getHeight();
    const pos_t width = arg.getData().getWidth();
    uint8_t *dest_buffer = tmp.getData().buffer;
    uint8_t *src_buffer = arg.getData().buffer;

    SUI_DEBUG_INFO("%ld", arg.getData().getScanLineSize());

    pos_t p = 0;
    for(pos_t y = 0; y < height; y++){
        for(pos_t x = 0; x < width; x++){
            memcpy(&dest_buffer[p], &src_buffer[pos + pos * y - x * 4 - 4], 4);
            p += 4;
        }
    }
    return tmp;
}


}
