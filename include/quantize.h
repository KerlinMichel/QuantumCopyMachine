#include <ctype.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <png.h>

void error(char *err_msg)
{
    printf("Error: %s\n", err_msg);
    abort();
}

typedef struct
{
    uint8_t red;
    uint8_t blue;
    uint8_t green;
} Pixel;

typedef struct
{
    Pixel **pixels;
    int width;
    int height;
} Image;

// http://www.libpng.org/pub/png/libpng-manual.txt
Image read_png_file(char *filename)
{
    FILE *fp = fopen(filename, "rb");

    png_structp png = png_create_read_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    if (!png)
    {
        error("Failed to create png struct");
    }

    png_infop info = png_create_info_struct(png);
    if (!info)
    {
        error("Failed to create png info struct");
    }

    if (setjmp(png_jmpbuf(png)))
    {
        error("setjmp/longjmp error");
    }

    png_init_io(png, fp);

    png_read_info(png, info);

    png_bytep *row_pointers = NULL;

    int width = png_get_image_width(png, info);
    int height = png_get_image_height(png, info);
    png_byte color_type = png_get_color_type(png, info);
    png_byte bit_depth = png_get_bit_depth(png, info);

    if (bit_depth == 16)
    {
        png_set_strip_16(png);
    }

    if (color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_palette_to_rgb(png);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY && bit_depth < 8)
    {
        png_set_expand_gray_1_2_4_to_8(png);
    }

    if (png_get_valid(png, info, PNG_INFO_tRNS))
    {
        png_set_tRNS_to_alpha(png);
    }

    // use filler=0xffff and PNG_FILLER_AFTER which will generate RGBA pixels.
    if (color_type == PNG_COLOR_TYPE_RGB ||
        color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_PALETTE)
    {
        png_set_filler(png, 0xFF, PNG_FILLER_AFTER);
    }

    if (color_type == PNG_COLOR_TYPE_GRAY ||
        color_type == PNG_COLOR_TYPE_GRAY_ALPHA)
    {
        png_set_gray_to_rgb(png);
    }

    png_read_update_info(png, info);

    if (row_pointers)
    {
        error("Row pointers is not null");
    }

    row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * height);
    for (int y = 0; y < height; y++)
    {
        row_pointers[y] = (png_byte *)malloc(png_get_rowbytes(png, info));
    }

    png_read_image(png, row_pointers);

    fclose(fp);

    png_destroy_read_struct(&png, &info, NULL);

    Pixel **pixels = (Pixel **)malloc(width * sizeof(Pixel *));

    for (int y = 0; y < width; y++)
    {
        pixels[y] = malloc(height * sizeof(Pixel));
    }

    for (int y = 0; y < height; y++)
    {
        png_bytep row = row_pointers[y];
        for (int x = 0; x < width; x++)
        {
            png_bytep px = &(row[x * 4]);
            Pixel pixel = {.red = px[0], .green = px[1], .blue = px[2]};
            pixels[x][y] = pixel;
        }
    }

    Image img = {.pixels = pixels, .width = width, .height = height};

    return img;
}

// returns the extension of a file name
char *get_filename_ext(char *filename)
{
    // get last dot character in filename
    char *dot_char = strrchr(filename, '.');
    // extension should be the characters after the last dot if the filename has an extension at the end
    char *ext_char = dot_char + 1;

    return ext_char;
}

int str_equals(const char *str1, const char *str2)
{
    return strcmp(str1, str2) == 0;
}

Image read_image(char *filename)
{
    char *ext = get_filename_ext(filename);

    const int ext_str_length = strlen(ext);
    char *ext_lower = (char *)malloc(ext_str_length + 1); // allocate 1 extra for null terminator
    ext_lower[ext_str_length] = 0;                        // null treminator

    for (int i = 0; i < ext_str_length; i++)
    {
        ext_lower[i] = tolower(ext[i]);
    }

    if (str_equals(ext_lower, "png"))
    {
        return read_png_file(filename);
    }
}
