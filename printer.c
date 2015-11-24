#include "printer.h"
#include <assert.h>
#include <stdlib.h>
#include <arpa/inet.h>
#include <sys/socket.h>
#include <string.h>
#include <unistd.h>
#include <stdio.h>
#include "error_private.h"
#include "constants.h"

#define STB_IMAGE_IMPLEMENTATION
#include "stb_image.h"

escpos_printer *escpos_printer_network(const char * const addr, const short port)
{
    assert(addr != NULL);

    int sockfd;
    escpos_printer *printer = NULL;

    sockfd = socket(AF_INET, SOCK_STREAM, 0);
    if (sockfd < 0) {
        last_error = ESCPOS_ERROR_SOCK;
    } else {
        struct sockaddr_in dest;
        dest.sin_family = AF_INET;
        dest.sin_port = htons(port);

        if (inet_pton(AF_INET, addr, &dest.sin_addr.s_addr) == 0) {
            last_error = ESCPOS_ERROR_INVALID_ADDR;
        } else if (connect(sockfd, (struct sockaddr*)&dest, sizeof(dest)) != 0) {
            last_error = ESCPOS_ERROR_CONNECTION_FAILED;
        } else {
            printer = (escpos_printer *)malloc(sizeof(escpos_printer));
            printer->sockfd = sockfd;
        }
    }

    return printer;
}

void escpos_printer_destroy(escpos_printer *printer)
{
    assert(printer != NULL);

    close(printer->sockfd);
    free(printer);
}

int escpos_printer_raw(escpos_printer *printer, const char * const message, const int len)
{
    assert(printer != NULL);

    int total = len;
    int sent = 0;
    int bytes = 0;

    // Make sure send() sends all data
    while (sent < total) {
        bytes = send(printer->sockfd, message, total, 0);
        if (bytes == -1) {
            last_error = ESCPOS_ERROR_SEND_FAILED;
            break;
        } else {
            sent += bytes;
        }
    }

    return !(sent == total);
}

int escpos_printer_cut(escpos_printer *printer)
{
    return escpos_printer_raw(printer, ESCPOS_CMD_CUT, 3);
}

int escpos_printer_feed(escpos_printer *printer, const unsigned char lines)
{
    char buffer[3];
    strncpy(buffer, ESCPOS_CMD_FEED, 2);
    buffer[2] = lines;
    return escpos_printer_raw(printer, buffer, sizeof(buffer));
}

void set_bit(unsigned char *byte, const int i, const int bit)
{
    assert(byte != NULL);
    assert(i >= 0 && i < 8);

    if (bit > 0) {
        *byte |= 1 << i;
    } else {
        *byte &= ~(1 << i);
    }
}

int convert_to_bit(const unsigned char *pixel, const int comp)
{
    assert(pixel != NULL);
    assert(comp > 0);

    int bit;

    switch (comp) {
    case ESCPOS_COMP_G:
    case ESCPOS_COMP_GA:
        bit = *pixel < 128;
        break;

    case ESCPOS_COMP_RGB:
    case ESCPOS_COMP_RGBA:
        {
            unsigned char r = *pixel;
            unsigned char g = *(pixel + 1);
            unsigned char b = *(pixel + 2);
            // Average the colors
            bit = ((r + g + b) / 3) < 128;
        }
        break;
    }

    return bit;
}

void convert_image_to_bits(unsigned char *pixel_bits, const unsigned char *image_data, const int w, const int h, const int comp)
{
    assert(pixel_bits != NULL);
    assert(image_data != NULL);

    for (int x = 0; x < w; x++) {
        for (int y = 0; y < h; y++) {
            int pi = x * h + y;
            int curr_byte = pi / 8;
            int bit = convert_to_bit(image_data + (y * w * comp) + (x * comp), comp);

            // The bit is set from left to right
            set_bit(&pixel_bits[curr_byte], 7 - (pi % 8), bit);
        }
    }
}

int escpos_printer_upload(escpos_printer *printer, const unsigned char *pixel_bits, const int w, const int h)
{
    assert(printer != NULL);
    assert(pixel_bits != NULL);
    assert(w > 0 && w <= ESCPOS_CHUNK_DOT_HEIGHT);
    assert(h > 0 && h <= ESCPOS_CHUNK_DOT_HEIGHT);
    assert(w % 32 == 0);
    assert(h % 32 == 0);

    // Header (4 bytes): [ESCPOS_CMD_DEFINE_DL_BIT_IMAGE] [byte_width] [byte_height]
    char buffer[4];
    strncpy(buffer, ESCPOS_CMD_DEFINE_DL_BIT_IMAGE, 2);
    buffer[2] = (unsigned char)(w / 8);
    buffer[3] = (unsigned char)(h / 8);

    int result = 0;
    if ((result = escpos_printer_raw(printer, buffer, 4)) == 0) {
        for (int i = 0; i < (w * (h / 8)) / 4; i++) {
            result = escpos_printer_raw(printer, (char *)(pixel_bits + (i * 4)), 4);
            if (result != 0) {
                break;
            }
        }
    }

    if (result != 0) {
        last_error = ESCPOS_ERROR_IMAGE_UPLOAD_FAILED;
    }

    return result;
}

int escpos_printer_print(escpos_printer *printer)
{
    assert(printer != NULL);

    int result = escpos_printer_raw(printer, ESCPOS_CMD_PRINT_DL_BIT_IMAGE, 3);

    if (result != 0) {
        last_error = ESCPOS_ERROR_IMAGE_PRINT_FAILED;
    }

    return result;
}

int escpos_printer_image(escpos_printer *printer, const char * const image_path)
{
    assert(printer != NULL);
    assert(image_path != NULL);

    int width, height, comp;
    int result = 0;

    unsigned char *image_data = stbi_load(image_path, &width, &height, &comp, 0);

    if (image_data != NULL) {
        int byte_width = ESCPOS_MAX_DOT_WIDTH / 8;
        int print_height = ESCPOS_CHUNK_DOT_HEIGHT - ESCPOS_CHUNK_OVERLAP;
        unsigned char pixel_bits[byte_width * ESCPOS_CHUNK_DOT_HEIGHT];

        int y = 0;
        while (y * print_height < height) {
            // Because the printer's image buffer has a limited memory,
            // if the image's height exceeds ESCPOS_CHUNK_DOT_HEIGHT pixels,
            // it is printed in chunks of x * ESCPOS_CHUNK_DOT_HEIGHT pixels.
            int chunk_height = (y + 1) * ESCPOS_CHUNK_DOT_HEIGHT <= height ? ESCPOS_CHUNK_DOT_HEIGHT : height - (y * ESCPOS_CHUNK_DOT_HEIGHT);
            convert_image_to_bits(pixel_bits, image_data + (y * print_height * width * comp), width, chunk_height, comp);
            if ((result = escpos_printer_upload(printer, pixel_bits, width, chunk_height)) == 0) {
                result = escpos_printer_print(printer);
            }

            if (result != 0) {
                break;
            }

            y += 1;
        }
    }

    return result;
}
