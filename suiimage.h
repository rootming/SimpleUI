#pragma once

#include "suitype.h"
#include "suisurface.h"

using namespace sui;
using namespace std;

namespace sui {

class SUIImage
{
public:
    SUIImage(SUISurface &surface);
    void save(const string &filename) const;

private:
    SUIData data;
    void savePPM(const string &filename) const;
};

}

