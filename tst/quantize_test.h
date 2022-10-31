#include <assert.h>

#include "quantize.h"


void test_read_png()
{
    Image img = read_png_file("./input_examples/images/Kenney_RPG_Urban_Tileset.png");

    // first pixel in top left is blank
    assert(img.pixels[0][0].red == 0);
    assert(img.pixels[0][0].green == 0);
    assert(img.pixels[0][0].blue == 0);

    // 5th pixel from top left is first non blank pixel
    assert(img.pixels[4][0].red == 131);
    assert(img.pixels[4][0].green == 106);
    assert(img.pixels[4][0].blue == 98);
}


void test_partition_image()
{
    Image img = read_png_file("./input_examples/images/Kenney_RPG_Urban_Tileset.png");

    Image *parted_images = partition_image(img, 16, 16);

    assert(img.pixels[0][0].red == parted_images[0].pixels[0][0].red);
    assert(img.pixels[0][0].blue == parted_images[0].pixels[0][0].blue);
    assert(img.pixels[0][0].green == parted_images[0].pixels[0][0].green);

    assert(img.pixels[0][16 * 2].red == parted_images[2].pixels[0][0].red);
    assert(img.pixels[0][16 * 2].blue == parted_images[2].pixels[0][0].blue);
    assert(img.pixels[0][16 * 2].green == parted_images[2].pixels[0][0].green);
}

