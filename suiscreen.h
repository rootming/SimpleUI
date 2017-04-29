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

    void refresh()
    {

        draw(this);
    }

    const SUIData & getData() const
    {
        return data;
    }

    void update()
    {
        SUI_DEBUG_WORRY("Surface width: %ld, height: %ld\n", data.getWidth(), data.getHeight());
        memcpy(fb_mmap, data.buffer, data.bytes());
    }

    uint8_t getRedOffset() const;

    uint8_t getGreenOffset() const;

    uint8_t getBlueOffset() const;

    uint8_t getAlphaOffset() const;

private:
    std::string deviceName;
    SUIData data;
    int fbdf;           //
    void *fb_mmap;		//内存地址
    uint8_t redOffset;
    uint8_t greenOffset;
    uint8_t blueOffset;
    uint8_t alphaOffset;

    void (*draw)(SUIScreen *screen);

    int init(const char *dev);



};

}

