#ifndef BASEDRAW_H
#define BASEDRAW_H
#include <iostream>
#include "suisurface.h"
#include "suitype.h"

using namespace sui;

namespace sui{

class SUIBaseDraw
{
public:
    SUIBaseDraw();
};

static inline void drawPixel(SUISurface &surface, int64_t x, int64_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
{
    size_t seek;
    int32_t tmp = 0;
    tmp = r << RED_SEEK | g << GREEN_SEEK | b << BLUE_SEEK | a << ALPHA_SEEK;
    seek = (x + surface.getData().getWidth() * y) * 3;
    //边缘检测
    //cout << "Draw postion:" << x << " " << y << endl;
//    //cout<< surface.width << " " << surface.height << endl;
//    if(x >= 0 && y >= 0 && x < surface.getData().getWidth() && y < surface.getData().getHeight()){
//        surface.getData().buffer[seek] = tmp;
//    }
    if(seek <= surface.getData().bytes()){
        //*((int32_t*)&(surface.getData().buffer[seek])) = tmp;
        memcpy(&surface.getData().buffer[seek], &tmp, sizeof(uint8_t) * 3);
    }
    else{
#ifdef SUI_DEBUG
        SUI_DEBUG_WORRY("Draw Pixel out of Range!\n");
#endif
    }
}


static inline void drawPixel(SUISurface &surface, const SUIPixel pixel)
{
    size_t seek;
    seek = (pixel.x + surface.getData().getWidth() * pixel.y) * 3;
    int32_t tmp = pixel.getColor();
    if(seek <= surface.getData().bytes()){
        //*((int32_t*)&(surface.getData().buffer[seek])) = pixel.getColor();
        memcpy(&surface.getData().buffer[seek], &tmp, sizeof(uint8_t) * 3);
    }
    else{
#ifdef SUI_DEBUG
        SUI_DEBUG_WORRY("Draw Pixel out of Range!\n");
#endif
    }
}


/* 画线函数 */
static inline void drawLine(SUISurface &surface, int64_t x1, int64_t y1, int64_t x2, int64_t y2,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
{
    int64_t start_x, end_x;
    int64_t start_y, end_y;
    SUIColor pixel(r, g, b, a);
    int32_t tmp = pixel.getColor();


    start_x = getMin(x1, x2);
    start_y = getMin(y1, y2);

    end_x = getMax(x1, x2);
    end_y = getMax(y1, y2);



    if(start_y == end_y) {
        size_t seek_start = (start_x + surface.getData().getWidth() * start_y) * 3;
        size_t seek_end = (end_x + surface.getData().getWidth() * start_y) * 3;
        while(seek_start <= seek_end) {
            memcpy(surface.getData().buffer + seek_start, &tmp, 3);
            seek_start += 3;
        }
    }

    else if(start_x == end_x) {
        size_t seek_start = (start_x + surface.getData().getWidth() * start_y) * 3;
        size_t seek_end = (start_x + surface.getData().getWidth() * end_y) * 3;
        while(seek_start <= seek_end) {
            memcpy(surface.getData().buffer + seek_start, &tmp, 3);
            seek_start += surface.getData().getWidth() * 3;
        }
    }

    else {

    }


}

static inline void drawLine(SUISurface &surface, SUIPost &post1, SUIPost &post2,
        SUIColor &color)
{
    drawLine(surface, post1.x, post1.y, post2.x, post2.y,
             color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

/* 画矩形函数 */
static inline void drawRect(SUISurface &surface,int64_t x, int64_t y,int64_t w, int64_t h,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
{
    drawLine(surface, x, y, x + w, y, r, g, b, a);
    drawLine(surface, x, y, x, y + h, r, g, b, a);
    drawLine(surface, x, y + h, x + w, y + h, r, g, b, a);
    drawLine(surface, x + w, y, x + w, y + h, r, g, b, a);
}

static inline void drawRectFill(SUISurface &surface, int64_t x,int64_t y,int64_t w,
        int32_t h, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0){}

static inline void drawRect(SUISurface &surface, SUIRect &rect, SUIColor &color){}

static inline void drawRectFill(SUISurface &surface,  SUIRect &rect, SUIColor &color){}

/* 画正方形函数 */
static inline void drawSquare(SUISurface &surface, int32_t x, int32_t y, int32_t len, uint8_t r,
        uint8_t g, uint8_t b, uint8_t a = 0){}
static inline void drawSquareFill(SUISurface &surface, int32_t x, int32_t y, int32_t len,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0){}

static inline void drawSquare(SUISurface &surface, int32_t x, int32_t y, int32_t len, SUIColor &color){}
static inline void drawSquareFill(SUISurface &surface, int32_t x, int32_t y, int32_t len, SUIColor &color){}


/* 画圆函数 */
static inline void drawCircle(SUISurface &surface, int32_t x0, int32_t y0, int32_t rad, uint8_t r,
        uint8_t g, uint8_t b, uint8_t a = 0){}
static inline void drawCircleFill(SUISurface &surface, int32_t x, int32_t y, int32_t rad,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0){}

/* 写字符函数,只支持ASCII */
static inline void drawStr(SUISurface &surface, std::string &str, int32_t x, int32_t y, SUIColor &color){}

static inline void drawStr(SUISurface &surface, std::string &str, SUIPost &post, SUIColor &color){}

static inline void fillSurface(SUISurface &surface, uint8_t r, uint8_t g, uint8_t b, uint8_t a){}

static inline void fillSurface(SUISurface &surface, SUIPixel &color){}

static inline void putDot(SUISurface &surface, int32_t x0, int32_t y0, int32_t x,
                   int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a){}

static inline void fillBottomFlatTriangle(SUISurface &surface, SUIPost &v1, SUIPost &v2, SUIPost &v3, SUIColor &color){}

static inline void fillTopFlatTriangle(SUISurface &surface, SUIPost &v1, SUIPost &v2, SUIPost &v3, SUIColor &color){}

static inline void drawTriangle(SUISurface &surface, SUIPost &post1, SUIPost &post2, SUIPost &post3, SUIColor &color){}

static inline void drawTriangleFill(SUISurface &surface, SUIPost &post1, SUIPost &post2, SUIPost &post3, SUIColor &color){}

}

#endif // SUIBASEDRAW_H
