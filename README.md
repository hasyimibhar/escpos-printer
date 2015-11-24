# ESC/POS Printer Library

This is a C library for operating thermal printers by sending ESC/POS commands.

## Compiling

```sh
make
```

This will create `dist` folder, which contains `libescposprinter.a` and some public header files.

## Sample Usage

This sample uses `stb_image.h` from [nothings/stb](https://github.com/nothings/stb):

```c
#include <stdio.h>
#include <string.h>
#include "escpos_printer.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

// Load and convert image to grayscale
unsigned char *load_image(const char * const image_path,
                          int *width,
                          int *height)
{
    int w, h, comp;
    unsigned char *image_data = stbi_load(image_path, &w, &h, &comp, 0);

    unsigned char *gs_image_data = (unsigned char *)malloc(sizeof(unsigned char) * w * h);
    if (comp == 1) {
        memcpy(gs_image_data, image_data, sizeof(unsigned char) * w * h);
    } else if (comp == 2) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                gs_image_data[y * w + x] = image_data[(y * w * 2) + (x * 2)];
            }
        }
    } else if (comp >= 3) {
        for (int y = 0; y < h; y++) {
            for (int x = 0; x < w; x++) {
                unsigned char r = image_data[(y * w * comp) + (x * comp)];
                unsigned char g = image_data[(y * w * comp) + (x * comp) + 1];
                unsigned char b = image_data[(y * w * comp) + (x * comp) + 2];

                gs_image_data[y * w + x] = (r + g + b) / 3;
            }
        }
    }

    stbi_image_free(image_data);

    *width = w;
    *height = h;
    return gs_image_data;
}

int main(int argc, char **argv)
{
    escpos_printer *printer = escpos_printer_network("192.168.0.123", 9100);

    if (printer != NULL) {
        int w, h;
        unsigned char *image_data = load_image("image.jpg", &w, &h);

        escpos_printer_image(printer, image_data, w, h);
        escpos_printer_feed(printer, 6);
        escpos_printer_cut(printer);

        free(image_data);
        escpos_printer_destroy(printer);
    } else {
        escpos_error err = escpos_last_error();
        printf("error: %d\n", err);
    }

    return 0;
}
```
