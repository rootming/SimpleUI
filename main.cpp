#include <iostream>
#include "suisurface.h"

using namespace std;

int main(int argc, char *argv[])
{
    SUISurface parent;
    SUIObject child;
    parent.addChild(&child);
    return 0;
}
