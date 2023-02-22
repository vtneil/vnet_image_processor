#include "image_processor.h"

using namespace vnet;

typedef Image<COLOR> image_t;

size_t w = 16;
size_t h = 16;
int q;

int main() {
    image_t img = image_t(w, h);
    uint8_t *buf;

    for (size_t i = 0; i < 8; ++i)
        for (size_t j = 0; j < 6; ++j)
            img.pixel(i + 5, j + 1).set(0xfe, 0x99, 0x1a);
    buf = img.serialize();

    printf("pxd:%ld\n", img.pixel(0, 0).size());
    printf("w:%ld h:%ld px:%ld buf:%ld\n", img.width(), img.height(), img.area(), img.buffer_size());

    img.pixel(5, 1).print();
    printf("\n");
    printi(img);
    printf("\n");
    image_t rx = image_t(w, h, buf);
    printi(rx);
    return 0;
}
