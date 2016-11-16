#pragma once

#include "suitype.h"
#include "suisurface.h"

using namespace sui;

namespace sui {

class SUIImage
{
public:
    SUIImage(SUISurface &surface);
    void save(const std::string &filename) const;

private:
    SUIData data;
    void savePPM(const std::string &filename) const;
};

}

