#include "suiscreen.h"
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

namespace sui {

static void drawPixel8(SUIScreen *screen)
{
    SUI_DEBUG_INFO("Not implement");
}

static void drawPixel16(SUIScreen *screen)
{
    auto child = screen->node.begin();

}

static void drawPixel24(SUIScreen *screen)
{

}

static void drawPixel32(SUIScreen *screen)
{

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

    SUI_DEBUG_INFO("Open Framebuffer Device:%s successful!\n", device);
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

    fb_mmap = mmap(NULL, fb_var.xres * fb_var.yres * data.depth, PROT_READ | PROT_WRITE,
                   MAP_SHARED, fbdf, 0);
    if(fb_mmap == NULL){
        SUI_DEBUG_ERROR("Memory Mmap error!\n");
        return EXIT_FAILURE;
    }

    SUI_DEBUG_INFO("Memory Mmap successful!\n");

    data.buffer = static_cast<uint8_t *>(fb_mmap);

    return EXIT_SUCCESS;
}



SUIScreen::~SUIScreen()
{
    if(fbdf != -1){
        munmap(fb_mmap, data.bytes());
        close(fbdf);
    }
}

}

