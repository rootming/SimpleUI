#include <iostream>
#include "sui.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[])
{
    SUIMain(argc, argv);
    SUIScreen screen("/dev/fb0");
    SUISurface parent(1000, 1000);
    SUIColor color(200, 200, 100);
    SUIColor color2(100, 200, 0);
    string str = "String print test.";
    screen.addChild(&parent);
//    for(int i = 0; i < 500; i++) {
//        for(int j = 0; j < 500; j++) {
//            drawPixel(parent, SUIPixel(i, j, 255, 255, 0));
//        }
//    }
    drawLine(parent, 20, 20, 100, 20, 255, 0, 0);
    drawLine(parent, 20, 20, 20, 100, 0, 255, 0);
    drawLine(parent, 20, 100, 100, 100, 0, 0, 255);
    drawLine(parent, 100, 20, 100, 100, 255, 255, 255);
    drawRect(parent, 200, 200, 50, 50, 255, 255, 255);
    drawRectFill(parent, 400, 400, 40, 40, 0, 255, 255);
    drawStr(parent, str, 10, 200, color);

    drawSquare(parent, 300, 300, 80, 255, 255, 255, 255);
    drawSquareFill(parent, 340, 340, 10, 100, 200, 200, 255);


    SUIPost post[3] = { {700, 800}, {800, 900}, {900, 800} };
    SUIPost post2[3] = { {300, 700}, {200, 800}, {100, 900} };
    drawTriangle(parent, post[0], post[1], post[2], color);
    drawTriangleFill(parent, post2[0], post2[1], post2[2], color2);
    SUIImage image(parent);

    int flag = -1;
    int step = 4;
    for(int i  = 0; i < parent.getData().getHeight(); i++) {
        if(i % step == 0)
            flag = -flag;
        if(flag > 0)
            drawPixel(parent, i, parent.getData().getWidth() / 2, 255, 0, 0);
    }

    flag = -1;
    for(int j  = 0; j < parent.getData().getWidth(); j++) {
        if(j % step == 0)
            flag = -flag;
        if(flag > 0)
            drawPixel(parent, parent.getData().getHeight() / 2, j, 255, 0, 0);
    }

    for(float y = -4.5f; y < 4.5f; y += 0.1f) {
        for(float x = -4.5f; x < 4.5f; x += 0.1f) {
            float a = x * x + y * y - 1;
            if( a * a * a - x * x * y * y * y < 0.0f) {
                drawPixel(parent, 100 + 10 * x, 600 + 10 * y, 255, 0, 0);
            }
        }

    }

    screen.refresh();
    image.save("Test.ppm");
    SUISurfaceUtility utility;
    SUISurface mir = utility.mirror(parent);
    SUIImage m(mir);
    m.save("mirror.ppm");

    return 0;
}
