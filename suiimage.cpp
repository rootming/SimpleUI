#include "suiimage.h"
#include <fstream>

using namespace std;

namespace sui {

SUIImage::SUIImage(SUISurface &surface)
{
    data = surface.getData();

}

void SUIImage::save(const char *name) const
{
    ofstream file(name, ios::binary | ios::trunc | ios::out);
    file << "P6\n" << data.getWidth() << " " << data.getHeight() << "\n255\n";
    file.write((const char *)data.buffer, data.bytes());
    file.close();

}

}
