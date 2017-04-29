#include "suiscreen.h"
#include "suisurface.h"

#include <unistd.h>
#include <fcntl.h>
#include <assert.h>
#include <time.h>
#include <math.h>
#include <sys/ioctl.h>
#include <sys/mman.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <linux/fb.h>
#include "suitype.h"


namespace sui {

static void drawPixel8(SUIScreen *screen)
{
    SUI_DEBUG_INFO("Not implement");
}

static void drawPixel16(SUIScreen *screen)
{
    SUISurface *surface = static_cast<SUISurface *>(screen->getChild(0));
    SUIScreen *device = static_cast<SUIScreen *>(screen);

    uint8_t *sourceBuffer = surface->getData().buffer;
    uint8_t *deviceBuffer = device->getData().buffer;
    uint16_t pixel;

    pos_t sw, sh, dw, dh;
    sw = surface->getData().getWidth();
    sh = surface->getData().getHeight();

    dw = device->getData().getWidth();
    dh = device->getData().getHeight();

    pos_t hl, wl;
    hl = getMin(sh, dh);
    wl = getMin(sw, dw);

    //SUI_DEBUG_INFO("Limit: %ld, %ld\n", wl, hl);

    for(pos_t y = 0; y < hl; y++) {
        for(pos_t x = 0; x < wl; x++) {
            pixel = SUIColor::toRGB565(*(int *)(sourceBuffer + (x + y * sw) * 4));
            memcpy(deviceBuffer + ((x + y * dw) * 2), (char *)&pixel, 2);
        }
    }

    device->update();

}

static void drawPixel24(SUIScreen *screen)
{
    SUI_DEBUG_INFO("Not implement");
}

static void drawPixel32(SUIScreen *screen)
{
    SUI_DEBUG_INFO("32 bpp Draw\n");
    SUISurface *surface = static_cast<SUISurface *>(screen->getChild(0));
    SUIScreen *device = static_cast<SUIScreen *>(screen);

    uint8_t *sourceBuffer = surface->getData().buffer;
    uint8_t *deviceBuffer = device->getData().buffer;
    uint32_t pixel;

    pos_t sw, sh, dw, dh;
    sw = surface->getData().getWidth();
    sh = surface->getData().getHeight();

    dw = device->getData().getWidth();
    dh = device->getData().getHeight();

    pos_t hl, wl;
    hl = getMin(sh, dh);
    wl = getMin(sw, dw);

    uint8_t rs, gs, bs;
    rs = screen->getRedOffset();
    gs = screen->getGreenOffset();
    bs = screen->getBlueOffset();


    //SUI_DEBUG_INFO("Limit: %ld, %ld\n", wl, hl);
    //SUI_DEBUG_INFO("Offset: %d, %d, %d\n", rs, gs, bs);


    for(pos_t y = 0; y < hl; y++) {
        for(pos_t x = 0; x < wl; x++) {
            pixel = SUIColor::toRGB888(*(int *)(&(sourceBuffer[(x + y * sw) * 4])), rs, gs, bs);
            memcpy(&deviceBuffer[y * device->getData().scanLineSize+ x * 4], &pixel, 4);
        }
    }

    device->update();
}


SUIScreen::SUIScreen(std::string dev): SUIObject(nullptr)
{

    init(dev.c_str());
}

int SUIScreen::init(const char *dev)
{

    struct fb_fix_screeninfo fb_fix;
    struct fb_var_screeninfo fb_var;

    if((fbdf = open(dev, O_RDWR)) < 0){
        SUI_DEBUG_ERROR("Open Framebuffer Device:%s error!\n", dev);
        return EXIT_FAILURE;
    }

    SUI_DEBUG_INFO("Open Framebuffer Device:%s successful!\n", dev);
    if(ioctl(fbdf, FBIOGET_FSCREENINFO, &fb_fix) == -1){
        SUI_DEBUG_ERROR("Get Fb_fix_info error!\n");
        return EXIT_FAILURE;
    }

    SUI_DEBUG_INFO("Get Fb_fix_info successful!\n");
    if(ioctl(fbdf, FBIOGET_VSCREENINFO, &fb_var) == -1){
        SUI_DEBUG_ERROR("Get Fb_var_info error!\n");
        return EXIT_FAILURE;
    }

    SUI_DEBUG_INFO("Get Fb_var_info successful!\n");
    //    fb_mmap = mmap(NULL, bytes(), PROT_READ | PROT_WRITE,
    //                          MAP_SHARED, fbdf, 0);


    switch(fb_var.bits_per_pixel)
    {
    case 32:
        data.depth = DEPTH32;
        draw = drawPixel32;
        break;
    case 24:
        data.depth = DEPTH24;
        draw = drawPixel24;
        break;
    case 16:
        data.depth = DEPTH16;
        draw = drawPixel16;
        break;
    case 8:
        data.depth = DEPTH8;
        draw = drawPixel8;
        break;
    default:
        SUI_DEBUG_ERROR("Not support depth");
        return EXIT_FAILURE;
        break;
    }

    data.w = fb_var.xres;
    data.h = fb_var.yres;
    data.scanLineSize = fb_fix.line_length;
    redOffset = fb_var.red.offset;
    blueOffset = fb_var.blue.offset;
    greenOffset = fb_var.green.offset;
    alphaOffset = fb_var.transp.offset;

    fb_mmap = mmap(NULL, data.bytes(), PROT_READ | PROT_WRITE,
                   MAP_SHARED, fbdf, 0);
    if(fb_mmap == NULL){
        SUI_DEBUG_ERROR("Memory Mmap error!\n");
        return EXIT_FAILURE;
    }

    SUI_DEBUG_INFO("Memory Mmap successful!\n");
    //SUI_DEBUG_INFO("Fbdev: %ld, %ld\n", data.w, data.h);


    data.buffer = new uint8_t[data.bytes()];

    return EXIT_SUCCESS;
}



SUIScreen::~SUIScreen()
{
    if(fbdf != -1){
        munmap(fb_mmap, data.bytes());
        close(fbdf);
    }
}

uint8_t SUIScreen::getRedOffset() const
{
    return redOffset;
}

uint8_t SUIScreen::getGreenOffset() const
{
    return greenOffset;
}

uint8_t SUIScreen::getBlueOffset() const
{
    return blueOffset;
}

uint8_t SUIScreen::getAlphaOffset() const
{
    return alphaOffset;
}

}

