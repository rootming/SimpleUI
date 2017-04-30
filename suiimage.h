#pragma once

#include "suitype.h"
#include "suisurface.h"

using namespace sui;
using namespace std;

namespace sui {

class SUIImage: public SUISurface
{
public:
    SUIImage(SUISurface *parent = nullptr);
    int loadImage(const string &filename);
    void save(const string &filename) const;

private:
    void savePPM(const string &filename) const;
};

}

