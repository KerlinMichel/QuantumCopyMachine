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

    for (int x = 0; x < width; x++)
    {
        pixels[x] = malloc(height * sizeof(Pixel));
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

Image* partition_image(Image image, int image_part_width, int image_part_height)
{
    int num_partition_rows = image.width / image_part_width;
    int num_partition_cols = image.height / image_part_height;

    int num_partitions = num_partition_rows * num_partition_cols;

    Image paritions[num_partitions];

    int partition_index = 0;
    for (int r = 0; r < num_partition_rows; r++)
    {
        for (int c = 0; c < num_partition_cols; c++)
        {
            // create parition pixels
            Pixel **pixels = (Pixel **)malloc(image_part_width * sizeof(Pixel *));

            for (int x = 0; x < image_part_width; x++)
            {
                // allocate memory
                pixels[x] = malloc(image_part_height * sizeof(Pixel));

                for (int y = 0; y < image_part_height; y++)
                {

                    // copy over pixels from original image to parition pixel
                    pixels[x][y] = image.pixels[(r * image_part_width) + x][(c * image_part_height) + y];

                    // create image and store in partitions
                    Image img = {.width = image_part_width, .height = image_part_height, .pixels = pixels};
                    paritions[partition_index] = img;
                }
            }
            partition_index++;
        }
    }

    Image *p = (Image *) paritions;

    return p;
}

void save_image(char *filename, Image image)
{
    FILE *fp = fopen(filename, "wb");

    png_structp png = png_create_write_struct(PNG_LIBPNG_VER_STRING, NULL, NULL, NULL);
    png_infop info = png_create_info_struct(png);

    png_init_io(png, fp);

    png_bytep *row_pointers = NULL;

    row_pointers = (png_bytep *)malloc(sizeof(png_bytep) * image.height);

    int pixel_size = 3;
    png_set_IHDR(png,
                 info,
                 image.width,
                 image.height,
                 8,
                 PNG_COLOR_TYPE_RGB,
                 PNG_INTERLACE_NONE,
                 PNG_COMPRESSION_TYPE_DEFAULT,
                 PNG_FILTER_TYPE_DEFAULT);

    png_byte color_type = png_get_color_type(png, info);

    row_pointers = png_malloc(png, image.height * sizeof (png_byte *));
    for (int y = 0; y < image.height; y++) {
        png_byte *row = png_malloc(png, sizeof (uint8_t) * image.width * pixel_size);
        row_pointers[y] = row;
        for (int x = 0; x < image.width; x++) {
            Pixel pixel = image.pixels[x][y];
            *row++ = pixel.red;
            *row++ = pixel.green;
            *row++ = pixel.blue;
        }
    }

    png_set_rows(png, info, row_pointers);
    png_write_png(png, info, PNG_TRANSFORM_IDENTITY, NULL);
    png_destroy_write_struct(&png, &info);
    fclose(fp);
}
