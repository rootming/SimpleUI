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
    case 24:
        data.depth = DEPTH24;
        break;
    case 16:
        data.depth = DEPTH24;
        break;
    case 8:
        data.depth = DEPTH24;
        break;
    case 1:
        data.depth = DEPTH24;
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

    fb_mmap = mmap(NULL, fb_var.xres * fb_var.yres * data.depth  / sizeof(char), PROT_READ | PROT_WRITE,
                   MAP_SHARED, fbdf, 0);
    if(fb_mmap == NULL){
        SUI_DEBUG_ERROR("Memory Mmap error!\n");
        return EXIT_FAILURE;
    }

    SUI_DEBUG_INFO("Memory Mmap successful!\n");

    return EXIT_SUCCESS;
}

}

