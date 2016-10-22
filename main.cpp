#include <iostream>
#include "sui.h"

using namespace std;
using namespace sui;

int main(int argc, char *argv[])
{
    SUISurface parent(500, 500);
    for(int i = 0; i < 500; i++){
        for(int j = 0; j < 500; j++){
            drawPixel(parent, SUIPixel(i, j, 255, 0, 0));
        }
    }
    SUIImage image(parent);
    image.save("Test.ppm");

    return 0;
}
