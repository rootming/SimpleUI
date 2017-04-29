#include "suiimage.h"
#include <fstream>
//#include <regex>

using namespace std;

namespace sui {



SUIImage::SUIImage(SUISurface &surface)
{
    data = surface.getData();

}

void SUIImage::savePPM(const string &filename) const
{
    char *buffer = new char[data.getWidth() * data.getHeight() * 3];
    char *p = buffer;
    ofstream file(filename.c_str(), ios::binary | ios::trunc | ios::out);
    file << "P6\n" << data.getWidth() << " " << data.getHeight() << "\n255\n";

    for(len_t i = 0; i < data.bytes(); i += 4) {
        memcpy(p, data.buffer + i, 3);
        p += 3;
    }

    file.write((const char *)buffer, data.getWidth() * data.getHeight() * 3);
    file.close();
}

void SUIImage::save(const string &filename) const
{
    savePPM(filename);
    //enum TYPE { JPG = 0, PNG, BMP, PPM };
    //regex pattern("[Jj][Pp][Ee]{0,1}[Gg]");

}

}
