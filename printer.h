#ifndef ESCPOSPRINTER_PRINTER_H
#define ESCPOSPRINTER_PRINTER_H

typedef struct escpos_config {
    // See ESCPOS_MAX_DOT_WIDTH in constants.h
    // Default value: ESCPOS_MAX_DOT_WIDTH
    int max_width;

    // See ESCPOS_CHUNK_DOT_HEIGHT in constants.h
    // Default value: ESCPOS_CHUNK_DOT_HEIGHT
    int chunk_height;

    unsigned int is_network_printer : 1;
} escpos_config;

typedef struct escpos_printer {
    int sockfd;
    escpos_config config;
} escpos_printer;

// Connects to an ESC/POS printer via network
//
// Params:
// - addr: the printer's address
// - port: the printer's port
//
// Return value: the printer object if successful, NULL otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern escpos_printer *escpos_printer_network(const char * const addr, const short port);

// Connects to an ESC/POS printer via serial
//
// Params:
// - portname: the path to the serial file to be used
// - baudrate: the baudrate for serial communication
//
// Return value: the printer object if successful, NULL otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern escpos_printer *escpos_printer_serial(const char * const portname, const int baudrate);

// Modifies the printer's configuration
//
// Params:
// - printer: the printer
// - config: the config
//
// Return value: 0 is successful, non-zero otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern int escpos_printer_config(escpos_printer *printer, const escpos_config * const config);

// Destroys the printer and deallocates its memory
//
// Params:
// - printer: the printer
extern void escpos_printer_destroy(escpos_printer *printer);

// Sends raw data to the printer
//
// Params:
// - printer: the printer
// - message: the data
// - len: the length of the data in bytes
//
// Return value: 0 is successful, non-zero otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern int escpos_printer_raw(escpos_printer *printer, const char * const message, const int len);

// Cuts the paper
//
// Params:
// - printer: the printer
// - lines: no. of lines to feed before cutting
//
// Return value: 0 is successful, non-zero otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern int escpos_printer_cut(escpos_printer *printer, const int lines);

// Feeds n lines
//
// Params:
// - printer: the printer
// - lines: no. of lines
//
// Return value: 0 is successful, non-zero otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern int escpos_printer_feed(escpos_printer *printer, const int lines);

// Prints an image
//
// NOTE: This function will send the image data to the printer's buffer
// before sending the print command, instead of printing the image directly.
//
// Params:
// - printer: the printer
// - image_data: an array of width * height bytes containing the pixels in grayscale
// - width: the image width (must be at most 512)
// - height: the image height
//
// Return value: 0 is successful, non-zero otherwise.
// If it fails, use escpos_last_error() to get the error code.
extern int escpos_printer_image(escpos_printer *printer,
                                const unsigned char * const image_data,
                                const int width,
                                const int height);

#endif
