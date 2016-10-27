#ifndef BASETYPE_H
#define BASETYPE_H
#include <inttypes.h>
#include <cmath>
#include <cstddef>
#include <string.h>
#include "suiconfig.h"

namespace sui {



typedef enum { DEPTH1, DEPTH8, DEPTH16, DEPTH24 } SUIDEPTH;

//#define GETMAX(a, b) (a) >= (b) ? (a) : (b)
//#define GETMIN(a, b) (a) <= (b) ? (a) : (b)
#ifdef SUI_BIG_ENDIAN
typedef enum { ALPHA_SEEK = 0, RED_SEEK = 24, GREEN_SEEK = 16, BLUE_SEEK = 8  } COLOR_SEEK;
#else
typedef enum { ALPHA_SEEK = 24, RED_SEEK = 0, GREEN_SEEK = 8, BLUE_SEEK = 16 } COLOR_SEEK;
#endif


template<typename T>
T& getMax(T &a, T &b)
{
    return a >= b ? a : b;
}

template<typename T>
T& getMin(T &a, T &b)
{
    return a <= b ? a : b;
}

struct SUIPost
{
    SUIPost(int32_t x, int32_t y)
    {
        this->x = x;
        this->y = y;
    }

    int32_t x;
    int32_t y;
    //int32_t seek;
};

struct SUIRect
{
    SUIRect(int32_t x = 0, int32_t y = 0, int32_t w = 0, int32_t h = 0)
    {
        this->x = x;
        this->y = y;
        this->w = w;
        this->h = h;
    }

    SUIRect(SUIPost pos1, SUIPost pos2)
    {
        this->x = getMin(pos1.x, pos2.x);
        this->y = getMin(pos1.y, pos2.y);
        this->w = std::abs(static_cast<long double>(pos1.x - pos2.x));
        this->h = std::abs(static_cast<long double>(pos1.y - pos2.y));
    }

    int32_t getWidth(void) const { return w; }
    int32_t getHeight(void) const { return h; }

    void setWidth(const int32_t width) { w = width; }
    void setHeight(const int32_t height) { h = height; }

    int32_t x;
    int32_t y;
    int32_t w;
    int32_t h;
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
    size_t bytes(void) const { return w * h * sizeof(uint8_t) * 3; }

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

    SUIColor(uint32_t color)
    {
        pixel = color;
    }

    SUIColor(uint16_t color)
    {
        pixel = color;
    }

    SUIColor(uint8_t color)
    {
        pixel = color;
    }

    void setColorMode(SUIDEPTH mode) { depth = mode; }
    void setColor(const int32_t color){ pixel = color; }
    void setRed(const uint8_t value) { pixel |= value << RED_SEEK; }
    void setGreen(const uint8_t value) { pixel |= value << GREEN_SEEK; }
    void setBlue(const uint8_t value) { pixel |= value << BLUE_SEEK; }
    void setAlpha(const uint8_t value) { pixel |= value << ALPHA_SEEK; }

    int32_t getColor(void) const { return pixel; }
    uint8_t getRed(void) const { return pixel >> RED_SEEK & 0xFF; }
    uint8_t getGreen(void) const { return pixel >> GREEN_SEEK & 0xFF; }
    uint8_t getBlue(void) const { return pixel >> BLUE_SEEK & 0xFF; }
    uint8_t getAlpha(void) const { return pixel & 0xFF; }

    uint32_t pixel;
    SUIDEPTH depth;

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
    SUIPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
        :SUIPost(x, y), SUIColor(r, g, b, a) {}
};

}


#endif // BASETYPE_H
