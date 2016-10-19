#ifndef BASETYPE_H
#define BASETYPE_H
#include <inttypes.h>
#include <cmath>

typedef enum { DEPTH1, DEPTH8, DEPTH16, DEPTH24 } SUIDEPTH;

//#define GETMAX(a, b) (a) >= (b) ? (a) : (b)
//#define GETMIN(a, b) (a) <= (b) ? (a) : (b)

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
        this->w = std::abs(pos1.x - pos2.x);
        this->h = std::abs(pos1.y - pos2.y);
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
//        postx = 0;
//        posty = 0;
        buffer = nullptr;
    }

//    int getWidth(void) const { return width; }
//    int getHeight(void) const { return height; }
//    int getPostx(void) const { return postx; }
//    int getPosty(void) const { return posty; }
//    void setWidth(const int32_t w) { width = w; }
//    void setHeight(const int32_t h) { height = h; }
//    void setPostx(const int32_t x){ postx = x; }
//    void setPosty(const int32_t y){ posty = y; }
//    int32_t postx, posty;

    SUIDEPTH depth;
    int32_t *buffer;	//缓冲图层指针
    int32_t bytes(void) const { return w * h * sizeof(int32_t); }

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
        pixel = r << 24 | g << 16 | b << 8 | a;
    }
    void setColor(const int32_t color){ pixel = color; }
    void setRed(const uint8_t value) { pixel |= value << 24; }
    void setGreen(const uint8_t value) { pixel |= value << 16; }
    void setBlue(const uint8_t value) { pixel |= value << 8; }
    void setAlpha(const uint8_t value) { pixel |= value; }
    int32_t getColor(void) const { return pixel; }
    uint8_t getRed(void) const { return pixel >> 24 & 0xFF; }
    uint8_t getGreen(void) const { return pixel >> 16 & 0xFF; }
    uint8_t getBlue(void) const { return pixel >> 8 & 0xFF; }
    uint8_t getAlpha(void) const { return pixel & 0xFF; }

    int32_t pixel;
//    SUIColor() :r(0), g(0), b(0), a(0){}
//    uint8_t r;
//    uint8_t g;
//    uint8_t b;
//    uint8_t a;
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
//    tmp.pixel = ((color1.pixel >> 24 & 0xFF) + (color2.pixel >> 24 & 0xFF)) << 24 |
//           ((color1.pixel >> 16 & 0xFF) + (color2.pixel >> 16 & 0xFF)) << 16 |
//           ((color1.pixel >> 8 & 0xFF) + (color2.pixel >> 8 & 0xFF)) << 8 |
//           ((color1.pixel & 0xFF) + (color2.pixel & 0xFF));
    return tmp;
}

static inline SUIColor operator- (SUIColor &color1, SUIColor &color2)
{
    SUIColor tmp;
    tmp.pixel = ((color1.pixel >> 24 & 0xFF) - (color2.pixel >> 24 & 0xFF)) << 24 |
           ((color1.pixel >> 16 & 0xFF) - (color2.pixel >> 16 & 0xFF)) << 16 |
           ((color1.pixel >> 8 & 0xFF) - (color2.pixel >> 8 & 0xFF)) << 8;
           //((color1.pixel & 0xFF) - (color2.pixel & 0xFF));
    return tmp;
}

static inline SUIColor operator+ (SUIColor &color1, uint8_t value)
{
    SUIColor tmp;
    tmp.pixel = ((color1.pixel >> 24 & 0xFF) + value) << 24 |
           ((color1.pixel >> 16 & 0xFF) + value) << 16 |
           ((color1.pixel >> 8 & 0xFF) + value) << 8;// |
           //((color1.pixel & 0xFF) + value);
    return tmp;
}

static inline SUIColor operator- (SUIColor &color1, uint8_t value)
{
    SUIColor tmp;
    tmp.pixel = ((color1.pixel >> 24 & 0xFF) - value) << 24 |
           ((color1.pixel >> 16 & 0xFF) - value) << 16 |
           ((color1.pixel >> 8 & 0xFF) - value) << 8;
           //((color1.pixel & 0xFF) - value);
    return tmp;
}





struct SUIPixel:public SUIPost, SUIColor
{
    SUIPixel(int32_t x, int32_t y, uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0)
        :SUIPost(x, y), SUIColor(r, g, b, a) {}
};

#endif // BASETYPE_H
