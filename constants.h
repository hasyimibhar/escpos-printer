#ifndef ESCPOSPRINTER_CONSTANTS_H
#define ESCPOSPRINTER_CONSTANTS_H

static const char *ESCPOS_CMD_INIT = "\x1b\x40";
static const char *ESCPOS_CMD_DEFINE_DL_BIT_IMAGE = "\x1d\x2a";
static const char *ESCPOS_CMD_PRINT_DL_BIT_IMAGE = "\x1d\x2f";
static const char *ESCPOS_CMD_CUT = "\x1d\x56\x00";
static const char *ESCPOS_CMD_FEED = "\x1b\x64";

#endif
