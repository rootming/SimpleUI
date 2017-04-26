#pragma once

#include <inttypes.h>
#include <cmath>
#include <cstddef>
#include <string.h>
#include <algorithm>
#include "suiconfig.h"

using namespace std;

namespace sui {

typedef enum { DEPTH8 = 1, DEPTH16 = 2, DEPTH24 = 3, DEPTH32  = 4 } SUIDEPTH;
typedef int64_t pos_t;
typedef uint64_t len_t;

typedef int16_t color16_t;
typedef int32_t color32_t;

//#define GETMAX(a, b) (a) >= (b) ? (a) : (b)
//#define GETMIN(a, b) (a) <= (b) ? (a) : (b)
#ifdef SUI_BIG_ENDIAN
typedef enum { ALPHA_SEEK = 0, RED_SEEK = 24, GREEN_SEEK = 16, BLUE_SEEK = 8  } COLOR_SEEK;
#else
typedef enum { ALPHA_SEEK = 24, RED_SEEK = 0, GREEN_SEEK = 8, BLUE_SEEK = 16 } COLOR_SEEK;
#endif


template<typename T>
inline T& getMax(T &a, T &b)
{
    return a >= b ? a : b;
}

template<typename T>
inline T& getMin(T &a, T &b)
{
    return a <= b ? a : b;
}

//template<typename T>
//inline void swap(T &a, T &b)
//{
//    T tmp = a;
//    b = a;
//    a = b;
//}

//template<int T>
//inline void swap(int &a, int &b)
//{
//    std::swap(a, b);
//}


struct SUIPost
{
    SUIPost(pos_t x, pos_t y)
    {
        this->x = x;
        this->y = y;
    }

    pos_t x;
    pos_t y;
    //pos_t seek;
};

struct SUIRect
{
    SUIRect(pos_t x = 0, pos_t y = 0, pos_t w = 0, pos_t h = 0)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
        setScanLineSize();
    }

    SUIRect(SUIPost pos1, SUIPost pos2)
    {
        this->x = getMin(pos1.x, pos2.x);
        this->y = getMin(pos1.y, pos2.y);
        this->w = abs(static_cast<long double>(pos1.x - pos2.x));
        this->h = abs(static_cast<long double>(pos1.y - pos2.y));
        setScanLineSize();
    }

    pos_t getX(void) const { return x; }
    pos_t getY(void) const { return y; }

    pos_t getWidth(void) const { return w; }
    pos_t getHeight(void) const { return h; }

    void setWidth(const pos_t width)
    {
        w = width;
        scanLineSize = w * 4;
    }

    void setHeight(const pos_t height)
    {
        h = height;
    }

    void setX(const pos_t post_x) { x = post_x; }
    void setY(const pos_t post_y) { y = post_y; }

    pos_t getScanLineSize(void) const
    {
        return scanLineSize;
    }

    virtual void setScanLineSize()
    {
    }

    pos_t scanLineSize;
    pos_t x;
    pos_t y;
    pos_t w;
    pos_t h;
};

struct SUIData: public SUIRect
{

    SUIData()
    {
        buffer = nullptr;
    }

    SUIData(const SUIData &orig): SUIRect(orig)
    {
        memcpy(buffer, orig.buffer, bytes());
    }

    SUIDEPTH depth;
    uint8_t *buffer;	//缓冲图层指针
    len_t bytes(void) const { return h * scanLineSize; }

    void setDepth(SUIDEPTH depth)
    {
        this->depth = depth;
    }

    virtual void setScanLineSize()
    {
        switch(depth) {
        case DEPTH32:
            scanLineSize = w * 4;
            break;
        case DEPTH24:
            scanLineSize = w * 3;
            break;
        case DEPTH16:
            scanLineSize = w * 2;
            break;
        case DEPTH8:
            scanLineSize = w * 1;
            break;

        }
    }

};


struct SUIColor
{
    SUIColor()
    {
        pixel = 0;
    }

    SUIColor(uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
    {
        pixel = 0;
        pixel = r << RED_SEEK | g << GREEN_SEEK | b << BLUE_SEEK | a << ALPHA_SEEK;
    }

    SUIColor(color32_t color)
    {
        pixel = color;
    }

    void setColor(const color32_t color) { pixel = color; }
    void setRed(const uint8_t value) { pixel |= value << RED_SEEK; }
    void setGreen(const uint8_t value) { pixel |= value << GREEN_SEEK; }
    void setBlue(const uint8_t value) { pixel |= value << BLUE_SEEK; }
    void setAlpha(const uint8_t value) { pixel |= value << ALPHA_SEEK; }

    color32_t getColor(void) const { return pixel; }
    uint8_t getRed(void) const { return pixel >> RED_SEEK & 0xFF; }
    uint8_t getGreen(void) const { return pixel >> GREEN_SEEK & 0xFF; }
    uint8_t getBlue(void) const { return pixel >> BLUE_SEEK & 0xFF; }
    uint8_t getAlpha(void) const { return pixel & 0xFF; }

    color32_t pixel;

    color16_t toRGB565()
    {
        return (pixel & 0xf80000) >> 8 | ((pixel & 0xfc00) >> 5) | ((pixel & 0xf8) >> 3);
    }


    static color16_t toRGB565(color32_t color)
    {
        return (color & 0xf80000) >> 8 | ((color & 0xfc00) >> 5) | ((color & 0xf8) >> 3);
    }


} ;

static inline SUIColor operator+ (SUIColor &front, SUIColor &back)
{
    SUIColor tmp;
    if(front.getAlpha() == 0) {
        return front;
    }
    else{
        float present_back = (255 - front.getAlpha()) / 255;
        float present_front = front.getAlpha() / 255;
        tmp.setRed(present_back * back.getRed() +  present_front * front.getRed());
        tmp.setGreen(present_back * back.getGreen() + present_front * front.getGreen());
        tmp.setBlue(present_back * back.getBlue() + present_front * front.getBlue());
    }
//    tmp.pixel = ((color1.pixel >> RED_SEEK & 0xFF) + (color2.pixel >> RED_SEEK & 0xFF)) << RED_SEEK |
//           ((color1.pixel >> GREEN_SEEK & 0xFF) + (color2.pixel >> GREEN_SEEK & 0xFF)) << GREEN_SEEK |
//           ((color1.pixel >> BLUE_SEEK & 0xFF) + (color2.pixel >> BLUE_SEEK & 0xFF)) << BLUE_SEEK |
//           ((color1.pixel & 0xFF) + (color2.pixel & 0xFF));
    return tmp;
}

static inline SUIColor operator- (SUIColor &color1, SUIColor &color2)
{
    SUIColor tmp;
    tmp.pixel = ((color1.pixel >> RED_SEEK & 0xFF) - (color2.pixel >> RED_SEEK & 0xFF)) << RED_SEEK |
           ((color1.pixel >> GREEN_SEEK & 0xFF) - (color2.pixel >> GREEN_SEEK & 0xFF)) << GREEN_SEEK |
           ((color1.pixel >> BLUE_SEEK & 0xFF) - (color2.pixel >> BLUE_SEEK & 0xFF)) << BLUE_SEEK;
           //((color1.pixel & 0xFF) - (color2.pixel & 0xFF));
    return tmp;
}

static inline SUIColor operator+ (SUIColor &color, uint8_t value)
{
    SUIColor tmp;
    tmp.pixel = ((color.pixel >> RED_SEEK & 0xFF) + value) << RED_SEEK |
           ((color.pixel >> GREEN_SEEK & 0xFF) + value) << GREEN_SEEK |
           ((color.pixel >> BLUE_SEEK & 0xFF) + value) << BLUE_SEEK;// |
           //((color1.pixel & 0xFF) + value);
    return tmp;
}

static inline SUIColor operator+ (uint8_t value, SUIColor &color)
{
    SUIColor tmp;
    tmp.pixel = ((color.pixel >> RED_SEEK & 0xFF) + value) << RED_SEEK |
           ((color.pixel >> GREEN_SEEK & 0xFF) + value) << GREEN_SEEK |
           ((color.pixel >> BLUE_SEEK & 0xFF) + value) << BLUE_SEEK;// |
           //((color1.pixel & 0xFF) + value);
    return tmp;
}

static inline SUIColor operator- (SUIColor &color1, uint8_t value)
{
    SUIColor tmp;
    tmp.pixel = ((color1.pixel >> RED_SEEK & 0xFF) - value) << RED_SEEK |
           ((color1.pixel >> GREEN_SEEK & 0xFF) - value) << GREEN_SEEK |
           ((color1.pixel >> BLUE_SEEK & 0xFF) - value) << BLUE_SEEK;
           //((color1.pixel & 0xFF) - value);
    return tmp;
}

struct SUIPixel:public SUIPost, SUIColor
{
    SUIPixel(pos_t x, pos_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
        :SUIPost(x, y), SUIColor(r, g, b, a) {}
};

}



