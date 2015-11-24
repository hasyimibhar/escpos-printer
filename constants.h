#ifndef ESCPOSPRINTER_CONSTANTS_H
#define ESCPOSPRINTER_CONSTANTS_H

static const char *ESCPOS_CMD_INIT = "\x1b\x40";
static const char *ESCPOS_CMD_DEFINE_DL_BIT_IMAGE = "\x1d\x2a";
static const char *ESCPOS_CMD_PRINT_DL_BIT_IMAGE = "\x1d\x2f\x00";
static const char *ESCPOS_CMD_CUT = "\x1d\x56\x00";
static const char *ESCPOS_CMD_FEED = "\x1b\x64";

// The maximum width of image the printer can accept
static const int ESCPOS_MAX_DOT_WIDTH = 512;

// When printing, if the image is too long, the printer
// will cut the images into chunks of (w x ESCPOS_CHUNK_DOT_HEIGHT)
static const int ESCPOS_CHUNK_DOT_HEIGHT = 512;

// Due to mechanical inaccuracy, when printing multiple
// chunks successively, a few rows of pixels from the next
// chunk will overlap the end of the previous chunk, causing
// an artifact. To fix this, when printing the next chunk,
// the printer will include the previous ESCPOS_CHUNK_OVERLAP
// rows of pixels.
static const int ESCPOS_CHUNK_OVERLAP = 2;

enum {
    ESCPOS_COMP_G = 1,
    ESCPOS_COMP_GA = 2,
    ESCPOS_COMP_RGB = 3,
    ESCPOS_COMP_RGBA = 4
};

#endif
