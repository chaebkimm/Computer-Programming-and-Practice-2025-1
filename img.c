#include <stdio.h>
#include <stdlib.h>
#include "png.h"

static unsigned char* buffer = NULL;
static unsigned int pos = 0;
static unsigned int buffer_size = 0;

int make_png_from_cifar_100() {
    FILE* cifar = fopen("./cifar-100-binary./train.bin", "rb");

    if (!cifar) return 1;

    unsigned char c;

    c = fgetc(cifar);
    printf("%d", c);

    c = fgetc(cifar);
    printf("%d", c);

    unsigned char rgbs[32 * 32 * 3];

    for (int i = 0; i < 32 * 32; i++) {
        rgbs[i * 3] = fgetc(cifar);
    }

    for (int i = 0; i < 32 * 32; i++) {
        rgbs[i * 3 + 1] = fgetc(cifar);
    }

    for (int i = 0; i < 32 * 32; i++) {
        rgbs[i * 3 + 2] = fgetc(cifar);
    }

    png_save("test.png", 32, 32, rgbs);

    fclose(cifar);
    return 0;
}

void make_small_c_img() {
    unsigned char rgbs[4 * 3 * 3];

    unsigned char light_red[3] = { 0x55, 0x00, 0x00 };
    unsigned char white[3] = { 0xff, 0xff, 0xff };

    unsigned char c_pattern[4][3] = {
        {0, 1, 1},
        {1, 0, 0},
        {1, 0, 0},
        {0, 1, 1}
    };

    int pos = 0;
    for (int i = 0; i < 4; i++) {
        for (int j = 0; j < 3; j++) {
            unsigned char* color;
            if (c_pattern[i][j] == 0) color = white;
            else color = light_red;
            for (int n = 0; n < 3; n++) rgbs[pos++] = color[n];
        }
    }

    rgbs[0] = 255;
    rgbs[1] = 0;
    rgbs[2] = 0;

    for (int i = 0; i < 4 * 3 * 3; i++)
        printf("%d ", rgbs[i]);

    png_save("test.png", 3, 4, rgbs);
}

void minimal_img() {
    unsigned char rgbs[3 * 2 * 2];

    rgbs[0] = 200;
    rgbs[1] = 0;
    rgbs[2] = 0;

    rgbs[3] = 255;
    rgbs[4] = 0;
    rgbs[5] = 0;

    rgbs[6] = 100;
    rgbs[7] = 0;
    rgbs[8] = 0;

    rgbs[9] = 50;
    rgbs[10] = 0;
    rgbs[11] = 0;

    png_save("test.png", 2, 2, rgbs);
}

void pixel(int red, int green, int blue) {

    unsigned char rgbs[3];

    rgbs[0] = red & 0xff;
    rgbs[1] = green & 0xff;
    rgbs[2] = blue & 0xff;

    printf("pixel... red: %02x, green: %02x, blue: %02x\n", rgbs[0], rgbs[1], rgbs[2]);

    png_save("test.png", 1, 1, rgbs);
}

void grow_if_needed() {
    if (!buffer) {
        buffer_size = 1024;
        buffer = realloc(buffer, buffer_size);
    }
    while (pos + 2 >= buffer_size) {
        buffer_size *= 2;
        buffer = realloc(buffer, buffer_size);
    }
}

void add_pixel(int red, int green, int blue) {

    grow_if_needed();

    buffer[pos++] = red;
    buffer[pos++] = green;
    buffer[pos++] = blue;
}

void grow_if_less_than_size(int size) {
    if (!buffer) {
        buffer_size = 1024;
        buffer = realloc(buffer, buffer_size);
    }
    while (size > buffer_size) {
        buffer_size *= 2;
        buffer = realloc(buffer, buffer_size);
    }
}

void make_png(int width, int height) {

    int size = width * height * 3;

    grow_if_less_than_size(size);

    for (int i = pos; i < size; i++) {
        buffer[i] = 255;
    }

    png_save("test.png", width, height, buffer);
}
