#include <iostream>
#include "sui.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[])
{
    SUIMain(argc, argv);
    SUISurface parent(500, 500);
    SUIColor color(200, 200, 100);
    string str = "String print test.";
//    for(int i = 0; i < 500; i++){
//        for(int j = 0; j < 500; j++){
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
    SUIImage image(parent);
    image.save("Test.ppm");

    return 0;
}
