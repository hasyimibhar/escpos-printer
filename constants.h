#ifndef ESCPOSPRINTER_CONSTANTS_H
#define ESCPOSPRINTER_CONSTANTS_H

static const char *ESCPOS_CMD_INIT = "\x1b\x40";
static const char *ESCPOS_CMD_PRINT_RASTER_BIT_IMAGE = "\x1d\x76\x30\x00";
static const char *ESCPOS_CMD_CUT = "\x1d\x56\x42";
static const char *ESCPOS_CMD_FEED = "\x1b\x64";

// The maximum width of image the printer can accept
static const int ESCPOS_MAX_DOT_WIDTH = 576;

// When printing, if the image is too long, the printer
// will cut the images into chunks of (w x ESCPOS_CHUNK_DOT_HEIGHT)
static const int ESCPOS_CHUNK_DOT_HEIGHT = 512;

#endif
