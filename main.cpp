#include <iostream>
#include "sui.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[])
{
    SUIMain(argc, argv);

    SUIScreen screen("/dev/fb1");

    SUISurface parent(320, 240);
    SUIColor color(200, 200, 100);

    string str = "String print test.";

    screen.addChild(&parent);

    drawLine(parent, 20, 20, 100, 20, 255, 0, 0);
    drawLine(parent, 20, 20, 20, 100, 0, 255, 0);
    drawLine(parent, 20, 100, 100, 100, 0, 0, 255);
    drawLine(parent, 100, 20, 100, 100, 255, 255, 255);
    drawRect(parent, 20, 20, 50, 50, 255, 255, 255);


    drawRectFill(parent, 40, 40, 40, 40, 0, 255, 255);
    drawStr(parent, str, 10, 200, color);

    drawSquare(parent, 30, 30, 80, 255, 255, 255, 255);

    drawSquareFill(parent, 34, 34, 10, 100, 200, 200, 255);


    SUIImage image(&parent);

    int flag = -1;
    int step = 4;
    for(int i  = 0; i < parent.getData().getWidth(); i++) {
        if(i % step == 0)
            flag = -flag;
        if(flag > 0)
            drawPixel(parent, i, parent.getData().getHeight() / 2, 255, 0, 0);

    }

    flag = -1;
    for(int j  = 0; j < parent.getData().getHeight(); j++) {
        if(j % step == 0)
            flag = -flag;
        if(flag > 0)
            drawPixel(parent, parent.getData().getWidth() / 2, j, 255, 0, 0);
    }

    for(float y = -4.5f; y < 4.5f; y += 0.1f) {
        for(float x = -4.5f; x < 4.5f; x += 0.1f) {
            float a = x * x + y * y - 1;
            if( a * a * a - x * x * y * y * y < 0.0f) {
                drawPixel(parent, 100 + 10 * x, 100 + 10 * y, 255, 0, 0);
            }
        }

    }

    SUIImage test;
    test.loadImage("test.png");
    screen.addChild(&test);
    screen.refresh();
    image.save("Test.ppm");
    SUISurfaceUtility utility;
    SUISurface mir = utility.mirror(parent);
    SUIImage m(&mir);
    m.save("mirror.ppm");

    return 0;
}
