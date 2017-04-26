#pragma once

#include <vector>
#pragma once

#include "suitype.h"
#include "suiobject.h"
#include "suidebug.h"

namespace sui {


class SUIScreen : public SUIObject, SUIData
{
public:
    SUIScreen(std::string dev = "/dev/fb");
    ~SUIScreen();

    void update()
    {
        draw(this);
    }


private:
    std::string deviceName;
    SUIData data;
    int fbdf;           //
    void *fb_mmap;		//内存地址
    char *device;		//设备名
    int8_t redOffset;
    int8_t greenOffset;
    int8_t blueOffset;
    int8_t alphaOffset;

    void (*draw)(SUIScreen *screen);

    int init(const char *dev);

};

}

