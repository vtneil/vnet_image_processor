#include "image_processor.h"

using namespace vnet;

//typedef Image<GRAYSCALE> image_t;
typedef Image<COLOR> image_t;

size_t w = 32;
size_t h = 20;
int q;

/*
 * a1 =      0000 1100
 * a2 =      0000 1001
 * a1 << 4 = 1100 0000
 * a1 | a2 = 1100 1001
 *
 * a1 =      0000 1100
 * a2 =      0000 1001
 */

int main() {
    image_t tx = image_t(w, h);
    uint8_t *buf;

    for (size_t i = 0; i < 8; ++i)
        for (size_t j = 0; j < 6; ++j)
            tx.pixel(i + 5, j + 1).set(0xfe, 0x99, 0x1a);

    for (size_t i = 0; i < 3; ++i)
        for (size_t j = 0; j < 6; ++j)
            tx.pixel(i + 16, j + 4).set(0xfe, 0x99, 0x1a);

    printf("Bitmap Information\n");
    printf("channel:%ld bit depth:%d per channel\n", tx.pixel(0, 0).size(), tx.depth());
    printf("w:%ld h:%ld px:%ld buf:%ld bit\nSample Data ", tx.width(), tx.height(), tx.area(), 8 * tx.buffer_size());

    tx.pixel(5, 1).print();
    printf("\nOriginal Image\n");
    printi(tx);

    printf("\nDeserialized Image\n");
    buf = tx.serialize();
    image_t rx = image_t(w, h, buf);
    printi(rx);
    return 0;
}
