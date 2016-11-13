#ifndef BASEDRAW_H
#define BASEDRAW_H
#include <iostream>
#include "suisurface.h"
#include "suitype.h"
#include "font.h"

using namespace sui;

namespace sui{

class SUIBaseDraw
{
public:
    SUIBaseDraw();
};

static inline void drawPixel(SUISurface &surface, const pos_t x, const pos_t y,
                             const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    size_t seek;
    color24_t tmp = 0;
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
    color24_t tmp = pixel.getColor();
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
static inline void drawLine(SUISurface &surface, const pos_t x1, const pos_t y1, const pos_t x2, const pos_t y2,
        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    pos_t start_x, end_x;
    pos_t start_y, end_y;
    SUIColor pixel(r, g, b, a);
    color24_t tmp = pixel.getColor();


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

static inline void drawLine(SUISurface &surface, const SUIPost &post1, const SUIPost &post2,
        const SUIColor &color)
{
    drawLine(surface, post1.x, post1.y, post2.x, post2.y,
             color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

/* 画矩形函数 */
static inline void drawRect(SUISurface &surface, const pos_t x, const pos_t y, const pos_t w, const pos_t h,
        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    drawLine(surface, x, y, x + w, y, r, g, b, a);
    drawLine(surface, x, y, x, y + h, r, g, b, a);
    drawLine(surface, x, y + h, x + w, y + h, r, g, b, a);
    drawLine(surface, x + w, y, x + w, y + h, r, g, b, a);
}

static inline void drawRectFill(SUISurface &surface, const pos_t x, const pos_t y, const pos_t w, const pos_t h,
                                const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    for(int start = y; start < y + h; start++) {
        drawLine(surface, x, start, x + w, start, r, g, b, a);
    }


}

static inline void drawRect(SUISurface &surface, const SUIRect &rect, const SUIColor &color)
{
    drawRect(surface, rect.x, rect.y, rect.w, rect.y,
             color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

static inline void drawRectFill(SUISurface &surface, const SUIRect &rect, const SUIColor &color)
{
    drawRectFill(surface, rect.x, rect.y, rect.w, rect.h,
                 color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

/* 画正方形函数 */
//static inline void drawSquare(SUISurface &surface, int32_t x, int32_t y, int32_t len, uint8_t r,
//        uint8_t g, uint8_t b, uint8_t a = 0){}
//static inline void drawSquareFill(SUISurface &surface, int32_t x, int32_t y, int32_t len,
//        uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0){}

//static inline void drawSquare(SUISurface &surface, int32_t x, int32_t y, int32_t len, SUIColor &color){}
//static inline void drawSquareFill(SUISurface &surface, int32_t x, int32_t y, int32_t len, SUIColor &color){}


/* 画圆函数 */
static inline void drawCircle(SUISurface &surface, const pos_t x0, const pos_t y0, const int32_t rad,
                              const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{

}

static inline void drawCircleFill(SUISurface &surface, const pos_t x, const pos_t y, const int32_t rad,
        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{

}

/* 写字符函数,只支持ASCII */
static inline void drawStr(SUISurface &surface, const std::string &str, const pos_t x, const pos_t y, const SUIColor &color)
{

#ifdef DEBUG
    SUIDEBUG_INFO("%s\n", str.c_str());
#endif
    pos_t ox, oy;
    size_t seek;
    const size_t len = str.length();
    const pos_t width = 8;                  //define font width
    ox = x;
    oy = y;

    const char *tmp = str.c_str();
    const uint8_t red = color.getRed();
    const uint8_t green = color.getGreen();
    const uint8_t blue = color.getBlue();
    const uint8_t alpha = color.getAlpha();

    for(size_t c = 0; c < len; c++){
        seek = tmp[c] * 10;
        if(tmp[c] == '\n'){
            ox = x;
            oy += 10;
            continue;
        }
        for(uint32_t l = 0; l < 10; l++){
#ifdef SUI_BIG_ENDIAN

#else
            for(uint32_t i = 0; i < 8; i++){
                if((font6x8_bits[seek] >> i) & 0x1){
                    drawPixel(surface, ox - i, oy + l, red, green, blue, alpha);
                    //cout<<"*";
                }
//                else{
//                     cout<<" ";
//                }
                //cout<<((font_bits[seek] << i) & 0x1);
                //(font_bits[seek] >> i & 0x1) ? cout <<"1" : cout <<"0";
            }
#endif
            //cout<<font_bits[seek]<<":"<<seek<<endl;
            //cout<<'\n';
           // cout<<font_bits[seek]<<endl;
            seek++;
        }

        ox += width;
    }
}

static inline void drawStr(SUISurface &surface, const std::string &str, const SUIPost &post, const SUIColor &color)
{
    drawStr(surface, str, post.x, post.y, color);
}

static inline void fillSurface(SUISurface &surface, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    SUIColor color(r, g, b, a);
    color24_t  tmp = color.getColor();
    const pos_t width = surface.getData().getWidth();
    for(int y = 0; y < surface.getData().getHeight(); y++){
        for(int x = 0; x < surface.getData().getWidth(); x++) {
            memcpy(surface.getData().buffer + (y * width + x) * 3, &tmp, 3);
        }
    }
}

static inline void fillSurface(SUISurface &surface, const SUIPixel &color)
{
    fillSurface(surface, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

//static inline void putDot(SUISurface &surface, const pos_t x0, const pos_t y0, const pos_t x, const pos_t y,
//                          const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a){}

static inline void fillBottomFlatTriangle(SUISurface &surface, const SUIPost &v1, const SUIPost &v2, const SUIPost &v3, const SUIColor &color){}

static inline void fillTopFlatTriangle(SUISurface &surface, const SUIPost &v1, const SUIPost &v2, const SUIPost &v3, const SUIColor &color){}

static inline void drawTriangle(SUISurface &surface, const SUIPost &post1, const SUIPost &post2, const SUIPost &post3, const SUIColor &color){}

static inline void drawTriangleFill(SUISurface &surface, const SUIPost &post1, const SUIPost &post2, const SUIPost &post3, const SUIColor &color){}

}

#endif // SUIBASEDRAW_H
