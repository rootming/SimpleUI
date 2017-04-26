#pragma once

#include <iostream>
#include <algorithm>
#include "suisurface.h"
#include "suitype.h"
#include "font.h"
#include "suidebug.h"

using namespace sui;
using namespace std;

namespace sui{

class SUIBaseDraw
{
public:
    SUIBaseDraw();
};

static inline void drawPixel(SUISurface &surface, const pos_t x, const pos_t y,
                             const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    len_t seek;
    color32_t tmp = 0;
    tmp = r << RED_SEEK | g << GREEN_SEEK | b << BLUE_SEEK | a << ALPHA_SEEK;
    seek = (x + surface.getData().getWidth() * y) * 4;

    //边缘检测
    //cout << "Draw postion:" << x << " " << y << endl;
//    //cout<< surface.width << " " << surface.height << endl;
//    if(x >= 0 && y >= 0 && x < surface.getData().getWidth() && y < surface.getData().getHeight()) {
//        surface.getData().buffer[seek] = tmp;
//    }
    if(seek <= surface.getData().bytes()) {
        //*((int32_t*)&(surface.getData().buffer[seek])) = tmp;
        memcpy(&surface.getData().buffer[seek], &tmp, sizeof(uint8_t) * 4);
    }
    else{
#ifdef SUI_DEBUG
        SUI_DEBUG_WORRY("Draw Pixel out of Range!\n");
#endif
    }
}


static inline void drawPixel(SUISurface &surface, const SUIPixel pixel)
{
    len_t seek;
    seek = (pixel.x + surface.getData().getWidth() * pixel.y) * 4;
    color32_t tmp = pixel.getColor();
    if(seek <= surface.getData().bytes()) {
        //*((int32_t*)&(surface.getData().buffer[seek])) = pixel.getColor();
        memcpy(&surface.getData().buffer[seek], &tmp, sizeof(uint8_t) * 4);
    }
    else{
#ifdef SUI_DEBUG
        SUI_DEBUG_WORRY("Draw Pixel out of Range!\n");
#endif
    }
}


/* 画线函数 */

static inline void plot(SUISurface &surface, const pos_t x, const pos_t y, const pos_t reverse,
                        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    len_t seek;
    color32_t tmp = 0;
    tmp = r << RED_SEEK | g << GREEN_SEEK | b << BLUE_SEEK | a << ALPHA_SEEK;
    if (reverse)
        seek = (y + surface.getData().getWidth() * x) * 4;
    else
        seek = (x + surface.getData().getWidth() * y) * 4;

    //边缘检测
    //cout << "Draw postion:" << x << " " << y << endl;
//    //cout<< surface.width << " " << surface.height << endl;
//    if(x >= 0 && y >= 0 && x < surface.getData().getWidth() && y < surface.getData().getHeight()) {
//        surface.getData().buffer[seek] = tmp;
//    }
    if(seek <= surface.getData().bytes()) {
        //*((int32_t*)&(surface.getData().buffer[seek])) = tmp;
        memcpy(&surface.getData().buffer[seek], &tmp, sizeof(uint8_t) * 4);
    }
    else{
#ifdef SUI_DEBUG
        SUI_DEBUG_WORRY("Draw Pixel out of Range!\n");
#endif
    }
}

static inline void drawLine(SUISurface &surface, const pos_t px1, const pos_t py1, const pos_t px2, const pos_t py2,
        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
//    pos_t start_x, end_x;
//    pos_t start_y, end_y;
//    SUIColor pixel(r, g, b, a);
//    color24_t tmp = pixel.getColor();


//    start_x = getMin(x1, x2);
//    start_y = getMin(y1, y2);

//    end_x = getMax(x1, x2);
//    end_y = getMax(y1, y2);



//    if(start_y == end_y) {
//        len_t seek_start = (start_x + surface.getData().getWidth() * start_y) * 3;
//        len_t seek_end = (end_x + surface.getData().getWidth() * start_y) * 3;
//        while(seek_start <= seek_end) {
//            memcpy(surface.getData().buffer + seek_start, &tmp, 3);
//            seek_start += 3;
//        }
//    }

//    else if(start_x == end_x) {
//        len_t seek_start = (start_x + surface.getData().getWidth() * start_y) * 3;
//        len_t seek_end = (start_x + surface.getData().getWidth() * end_y) * 3;
//        while(seek_start <= seek_end) {
//            memcpy(surface.getData().buffer + seek_start, &tmp, 3);
//            seek_start += surface.getData().getWidth() * 3;
//        }
//    }

//    else {

//    }


    // Symmetric Double Step
    pos_t a1 = px1, b1 = py1, a2 = px2, b2 = py2;
    pos_t dx, dy, incr1, incr2, d, x, y, xend, c, pixels_left;
    pos_t x1, y1;
    pos_t sign_x, sign_y, step, reverse, i;

    sign_x = (a2 - a1) < 0 ? -1 : 1;
    dx = (a2 - a1) * sign_x;
    sign_y = (b2 - b1) < 0 ? -1 : 1;
    dy = (b2 - b1) * sign_y;
    // Decide increment sign by the slope sign
    if (sign_x == sign_y)
        step = 1;
    else
        step = -1;

    // Choose axis of greatest movement (make * dx)
    if (dy > dx) {
        swap(a1, b1);
        swap(a2, b2);
        swap(dx, dy);
        reverse = 1;
    } else
        reverse = 0;
    // Note! Error check for dx == 0 should be included here
    // Start from the smaller coordinate
    if (a1 > a2) {
        x = a2;
        y = b2;
        x1 = a1;
        y1 = b1;
    } else {
        x = a1;
        y = b1;
        x1 = a2;
        y1 = b2;
    }

    // Note! dx = n implies 0 - n or (dx + 1) pixels to be set
    // Go round loop dx / 4 times then plot last 0, 1, 2 or 3 pixels
    // In fact (dx - 1) / 4 as 2 pixels are already plotted
    xend = (dx - 1) / 4;
    // Number of pixels left over at the end
    pixels_left = (dx - 1) % 4;
    plot(surface, x, y, reverse, r, g, b, a);
    // Plot only one pixel for zero length vectors
    if (pixels_left < 0)
        return;

    // Plot first two pints
    plot(surface, x1, y1, reverse, r, g, b, a);
    incr2 = 4 * dy - 2 * dx;
    // Slope less than 1/2
    if (incr2 < 0) {
        c = 2 * dy;
        incr1 = 2 * c;
        d = incr1 - dx;

        for (i = 0; i < xend; i++) {	// Plotting loop
            ++x;
            --x1;
            if (d < 0) {
                // Pattern 1 forwards
                plot(surface, x, y, reverse, r, g, b, a);
                plot(surface, ++x, y, reverse, r, g, b, a);
                // Pattern 1 backwards
                plot(surface, x1, y1, reverse, r, g, b, a);
                plot(surface, --x1, y1, reverse, r, g, b, a);
                d += incr1;
            } else {
                if (d < c) {
                    // Pattern 2 forwards
                    plot(surface, x, y, reverse, r, g, b, a);
                    plot(surface, ++x, y += step, reverse, r, g, b, a);
                    // Pattern 2 backwards
                    plot(surface, x1, y1, reverse, r, g, b, a);
                    plot(surface, --x1, y1 -= step, reverse, r, g, b, a);
                } else {
                    // Pattern 3 forwards
                    plot(surface, x, y += step, reverse, r, g, b, a);
                    plot(surface, ++x, y, reverse, r, g, b, a);
                    // Pattern 3 backwards
                    plot(surface, x1, y1 -= step, reverse, r, g, b, a);
                    plot(surface, --x1, y1, reverse, r, g, b, a);
                }
                d += incr2;
            }
        }

        // Plot last pattern
        if (pixels_left) {
            if (d < 0) {
                // Pattern 1
                plot(surface, ++x, y, reverse, r, g, b, a);
                if (pixels_left > 1)
                    plot(surface, ++x, y, reverse, r, g, b, a);
                if (pixels_left > 2)
                    plot(surface, --x1, y1, reverse, r, g, b, a);
            } else {
                if (d < c) {
                    // Pattern 2
                    plot(surface, ++x, y, reverse, r, g, b, a);
                    if (pixels_left > 1)
                        plot(surface, ++x, y += step, reverse, r, g, b, a);
                    if (pixels_left > 2)
                        plot(surface, --x1, y1, reverse, r, g, b, a);
                } else {
                    // Pattern 3
                    plot(surface, ++x, y += step, reverse, r, g, b, a);
                    if (pixels_left > 1)
                        plot(surface, ++x, y, reverse, r, g, b, a);
                    if (pixels_left > 2)
                        plot(surface, --x1, y1 -= step, reverse, r, g, b, a);
                }
            }
        }
    } else {
        // Slope greater than 1/2
        c = 2 * (dy - dx);
        incr1 = 2 * c;
        d = incr1 + dx;

        for (i = 0; i < xend; i++) {	// Plotting loop
            ++x;
            --x1;
            if (d > 0) {
                // Pattern 4 forwards
                plot(surface, x, y += step, reverse, r, g, b, a);
                plot(surface, ++x, y += step, reverse, r, g, b, a);
                // Pattern 4 backwards
                plot(surface, x1, y1 -= step, reverse, r, g, b, a);
                plot(surface, --x1, y1 -= step, reverse, r, g, b, a);
                d += incr1;
            } else {
                if (d < c) {
                    // Pattern 2 forwards
                    plot(surface, x, y, reverse, r, g, b, a);
                    plot(surface, ++x, y += step, reverse, r, g, b, a);
                    // Pattern 2 backwards
                    plot(surface, x1, y1, reverse, r, g, b, a);
                    plot(surface, --x1, y1 -= step, reverse, r, g, b, a);
                } else {
                    // Pattern 3 forwards
                    plot(surface, x, y += step, reverse, r, g, b, a);
                    plot(surface, ++x, y, reverse, r, g, b, a);
                    // Pattern 3 backwards
                    plot(surface, x1, y1 -= step, reverse, r, g, b, a);
                    plot(surface, --x1, y1, reverse, r, g, b, a);
                }
                d += incr2;
            }
        }

        // Plot last pattern
        if (pixels_left) {
            if (d > 0) {
                // Pattern 4
                plot(surface, ++x, y += step, reverse, r, g, b, a);
                if (pixels_left > 1)
                    plot(surface, ++x, y += step, reverse, r, g, b, a);
                if (pixels_left > 2)
                    plot(surface, --x1, y1 -= step, reverse, r, g, b, a);
            } else {
                if (d < c) {
                    // Pattern 2
                    plot(surface, ++x, y, reverse, r, g, b, a);
                    if (pixels_left > 1)
                        plot(surface, ++x, y += step, reverse, r, g, b, a);
                    if (pixels_left > 2)
                        plot(surface, --x1, y1, reverse, r, g, b, a);
                } else {
                    // Pattern 3
                    plot(surface, ++x, y += step, reverse, r, g, b, a);
                    if (pixels_left > 1)
                        plot(surface, ++x, y, reverse, r, g, b, a);
                    if (pixels_left > 2) {
                        if (d > c)	// Step 3
                            plot(surface, --x1, y1 -= step, reverse, r, g, b, a);
                        else		// Step 2
                            plot(surface, --x1, y1, reverse, r, g, b, a);
                    }
                }
            }
        }
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
    for(pos_t start = y; start < y + h; start++) {
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
static inline void drawSquare(SUISurface &surface, int32_t x, int32_t y, int32_t len, uint8_t r,
        uint8_t g, uint8_t b, uint8_t a = 0) 
{
    drawLine(surface, x, y, x + len, y, r, g, b, a);
    drawLine(surface, x, y, x, y + len, r, g, b, a);
    drawLine(surface, x + len, y, x + len, y + len, r, g, b, a);
    drawLine(surface, x, y + len, x + len, y + len, r, g, b, a);
}

static inline void drawSquareFill(SUISurface &surface, int32_t x, int32_t y, int32_t len,
        uint8_t r, uint8_t g, uint8_t b, uint8_t a = 0) 
{

    for(pos_t start = y; start < y + len; start++) {
        drawLine(surface, x, start, x + len, start, r, g, b, a);
    }
}

static inline void drawSquare(SUISurface &surface, int32_t x, int32_t y, int32_t len, SUIColor &color)
{
    drawSquare(surface, x, y, len, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

}

static inline void drawSquareFill(SUISurface &surface, int32_t x, int32_t y, int32_t len, SUIColor &color)
{
    drawSquareFill(surface, x, y, len, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());

}


/* 画圆函数 */

static inline void putDot(SUISurface &surface, const pos_t x0, const pos_t y0, const pos_t x, const pos_t y,
                          const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
        drawPixel(surface, x0 + x, y0 + y, r, g, b, a);
        drawPixel(surface, x0 + x, y0 - y, r, g, b, a);
        drawPixel(surface, x0 - x, y0 + y, r, g, b, a);
        drawPixel(surface, x0 - x, y0 - y, r, g, b, a);
        drawPixel(surface, x0 + y, y0 + x, r, g, b, a);
        drawPixel(surface, x0 + y, y0 - x, r, g, b, a);
        drawPixel(surface, x0 - y, y0 + x, r, g, b, a);
        drawPixel(surface, x0 - y, y0 - x, r, g, b, a);
}

static inline void drawCircle(SUISurface &surface, const pos_t x0, const pos_t y0, const int32_t rad,
                              const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
    if(x0 - r >= surface.getData().getWidth() || y0 - r >= surface.getData().getHeight())
           return;
       pos_t x, y;
       float d;
       x = 0;
       y = rad;
       d = 5.0 / 4 - r;

       while(x <= y) {
           putDot(surface, x0, y0, x, y, r, g, b, a);
           if(d < 0)
               d += x * 2.0 + 1;
           else{
               d += 2.0 * (x - y) + 1;
               //d+=2.0*(x-y)+5;
               y--;
           }
           x++;
       }
}

static inline void drawCircleFill(SUISurface &surface, const pos_t x, const pos_t y, const int32_t rad,
        const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a = 0)
{
        pos_t yy;
        double tmp;
        for(yy = y - rad; yy <= y + rad; yy++) {
            tmp = sqrt(rad * rad - (yy - y) * (yy - y));
            drawLine(surface, x - tmp, yy, x + tmp, yy, r, g, b, a);
        }

}

/* 写字符函数,只支持ASCII */
static inline void drawStr(SUISurface &surface, const string &str, const pos_t x, const pos_t y, const SUIColor &color)
{

#ifdef DEBUG
    SUIDEBUG_INFO("%s\n", str.c_str());
#endif
    pos_t ox, oy;
    len_t seek;
    const len_t len = str.length();
    const pos_t width = 8;                  //define font width
    ox = x;
    oy = y;

    const char * const tmp = str.c_str();
    const uint8_t red = color.getRed();
    const uint8_t green = color.getGreen();
    const uint8_t blue = color.getBlue();
    const uint8_t alpha = color.getAlpha();

    for(len_t c = 0; c < len; c++) {
        seek = tmp[c] * 10;
        if(tmp[c] == '\n') {
            ox = x;
            oy += 10;
            continue;
        }
        for(uint32_t l = 0; l < 10; l++) {
#ifdef SUI_BIG_ENDIAN
            SUIDEBUG_WORRY("Not support!!!");
#else
            for(uint32_t i = 0; i < 8; i++) {
                if((font6x8_bits[seek] >> i) & 0x1) {
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

static inline void drawStr(SUISurface &surface, const string &str, const SUIPost &post, const SUIColor &color)
{
    drawStr(surface, str, post.x, post.y, color);
}

static inline void fillSurface(SUISurface &surface, const uint8_t r, const uint8_t g, const uint8_t b, const uint8_t a)
{
    SUIColor color(r, g, b, a);
    color32_t  tmp = color.getColor();
    const pos_t width = surface.getData().getWidth();
    for(int y = 0; y < surface.getData().getHeight(); y++) {
        for(int x = 0; x < surface.getData().getWidth(); x++) {
            memcpy(surface.getData().buffer + (y * width + x) * 4, &tmp, 4);
        }
    }
}

static inline void fillSurface(SUISurface &surface, const SUIPixel &color)
{
    fillSurface(surface, color.getRed(), color.getGreen(), color.getBlue(), color.getAlpha());
}

static inline void fillBottomFlatTriangle(SUISurface &surface, const SUIPost &post1, const SUIPost &post2, const SUIPost &post3, const SUIColor &color)
{
    pos_t invslope1 = (post2.x - post1.x) / (post2.y - post1.y);
    pos_t invslope2 = (post3.x - post1.x) / (post3.y - post1.y);

    pos_t curx1 = post1.x;
    pos_t curx2 = post1.x;

    const uint8_t red = color.getRed();
    const uint8_t green  = color.getGreen();
    const uint8_t blue  = color.getBlue();
    const uint8_t alpha = color.getAlpha();

    for (pos_t scanlineY = post1.y; scanlineY <= post2.y; scanlineY++) {
      drawLine(surface, curx1, scanlineY, curx2, scanlineY, red, green, blue, alpha);
      curx1 += invslope1;
      curx2 += invslope2;
    }
}

static inline void fillTopFlatTriangle(SUISurface &surface, const SUIPost &post1, const SUIPost &post2, const SUIPost &post3, const SUIColor &color)
{
    int32_t invslope1 = (post3.x - post1.x) / (post3.y - post1.y);
    int32_t invslope2 = (post3.x - post2.x) / (post3.y - post2.y);

    int32_t curx1 = post3.x;
    int32_t curx2 = post3.x;

    const uint8_t red = color.getRed();
    const uint8_t green  = color.getGreen();
    const uint8_t blue  = color.getBlue();
    const uint8_t alpha = color.getAlpha();

    for (int32_t scanlineY = post3.y; scanlineY > post1.y; scanlineY--) {
      curx1 -= invslope1;
      curx2 -= invslope2;
      drawLine(surface, curx1, scanlineY, curx2, scanlineY, red, green, blue, alpha);
    }
}

static inline void drawTriangle(SUISurface &surface, const SUIPost &post1, const SUIPost &post2, const SUIPost &post3, const SUIColor &color)
{
    drawLine(surface, post1, post2, color);
    drawLine(surface, post1, post3, color);
    drawLine(surface, post2, post3, color);
}

static inline void drawTriangleFill(SUISurface &surface, const SUIPost &post1, const SUIPost &post2, const SUIPost &post3, const SUIColor &color)
{
    vector<SUIPost> v;
    v.push_back(post1);
    v.push_back(post2);
    v.push_back(post3);
    sort(v.begin(), v.end(), [](const SUIPost &a, const SUIPost &b) { return a.y < b.y;});
   #ifdef DEBUG
       for(auto it:v)
           cout << it.x << ":" << it.y << endl;
       SUIDEBUG_INFO("Sort postition\n");
   #endif
    /* here we know that v1.y <= v2.y <= v3.y */
    /* check for trivial case of bottom-flat triangle */
    if (v[1].y == v[2].y) {
        fillBottomFlatTriangle(surface, v[0], v[1], v[2], color);
    }
    /* check for trivial case of top-flat triangle */
    else if (v[0].y == v[1].y) {
        fillTopFlatTriangle(surface, v[0], v[1], v[2], color);
    }
    else{
     //         (int)(vt1.x + ((float)(vt2.y - vt1.y) / (float)(vt3.y - vt1.y)) * (vt3.x - vt1.x)), vt2.y);
        /* general case - split the triangle in a topflat and bottom-flat one */
        SUIPost v4((v[0].x + ((v[1].y - v[0].y) / (v[2].y - v[0].y)) * (v[2].x - v[0].x)), v[1].y);
        fillBottomFlatTriangle(surface, v[0], v[1], v4, color);
        fillTopFlatTriangle(surface, v[1], v4, v[2], color);
    }

}

}


