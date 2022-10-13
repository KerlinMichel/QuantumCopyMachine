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

