#include "suiimage.h"
#include <fstream>
#include <png.h>
//#include <regex>

using namespace std;

namespace sui {



SUIImage::SUIImage(SUISurface *parent): SUISurface(parent)
{
    if(parent != nullptr)
        data = parent->getData();
}

int SUIImage::loadImage(const string &filename)
{


    pos_t seek = 0;
    int32_t tmp;

    FILE *fp = NULL;

    unsigned char *src = NULL;

    //Image* image = NULL;
    png_structp png_ptr = NULL;
    png_infop info_ptr = NULL;
    png_bytep * row_pointers = NULL;

    if((fp = fopen(filename.c_str(), "rb")) == NULL){
        printf("%s: open %s failed.\n", __func__, filename.c_str());
        return EXIT_FAILURE;
    }

    if((png_ptr = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL,
                                         NULL)) == NULL)
    {
        fclose(fp);
        return EXIT_FAILURE;
    }

    if((info_ptr = png_create_info_struct(png_ptr)) == NULL){
        fclose(fp);
        return EXIT_FAILURE;
    }

#ifdef PNG_SETJMP_SUPPORTED
    setjmp(png_jmpbuf(png_ptr));
#endif

    png_init_io(png_ptr, fp);

    memset(info_ptr, 0x00, sizeof(*info_ptr));
    png_read_info(png_ptr, info_ptr);

    resize(info_ptr->width, info_ptr->height);

    //SFGL_Surface *surface = CreatSurfaceRGBA(w, h);
    //	passes_nr = png_set_interlace_handling(png_ptr);
    png_set_interlace_handling(png_ptr);
    png_read_update_info(png_ptr, info_ptr);

#ifdef PNG_SETJMP_SUPPORTED
    setjmp(png_jmpbuf(png_ptr));
#endif

    row_pointers = (png_bytep*)calloc(1, data.getHeight() * sizeof(png_bytep));
    for(int ty = 0; ty < data.getHeight(); ty++){
        row_pointers[ty] = (png_byte*)calloc(1, info_ptr->rowbytes);
    }


    png_read_image(png_ptr, row_pointers);

    if(info_ptr->color_type == PNG_COLOR_TYPE_RGBA){
        for(int ty = 0; ty < data.getHeight(); ty++){
            src = row_pointers[ty];
            for(int x = 0; x < data.getWidth(); x++){
                tmp = src[0] << RED_SEEK |
                                src[1] << GREEN_SEEK |
                                          src[2] << BLUE_SEEK |
                                                    src[3] << ALPHA_SEEK;
                memcpy((int32_t *)data.buffer + seek, &tmp, 4);
                seek = x + ty * data.getWidth();
                src += 4;
            }
        }
    }


    //    #if PNG_LIBPNG_VER > 10399
    //                png_byte red = png_ptr->trans_color.red & 0xff;
    //                png_byte green = png_ptr->trans_color.green & 0xff;
    //                png_byte blue = png_ptr->trans_color.blue & 0xff;
    //    #else
    //                png_byte red = png_ptr->trans_values.red & 0xff;
    //                png_byte green = png_ptr->trans_values.green & 0xff;
    //                png_byte blue = png_ptr->trans_values.blue & 0xff;
    //    #endif


    for(int ty = 0; ty < data.getHeight(); ty++){
        free(row_pointers[ty]);
    }
    free(row_pointers);
    png_destroy_read_struct(&png_ptr, &info_ptr, NULL);

    fclose(fp);

    return EXIT_SUCCESS;
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
