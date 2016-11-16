#include "suiimage.h"
#include <fstream>
#include <regex>

using namespace std;

namespace sui {



SUIImage::SUIImage(SUISurface &surface)
{
    data = surface.getData();

}

void SUIImage::savePPM(const string &filename) const
{
    ofstream file(filename.c_str(), ios::binary | ios::trunc | ios::out);
    file << "P6\n" << data.getWidth() << " " << data.getHeight() << "\n255\n";
    file.write((const char *)data.buffer, data.bytes());
    file.close();
}

void SUIImage::save(const string &filename) const
{
    //enum TYPE { JPG = 0, PNG, BMP, PPM };
    //regex pattern("[Jj][Pp][Ee]{0,1}[Gg]");

}

}
