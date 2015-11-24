#ifndef ESCPOSPRINTER_PRINTER_H
#define ESCPOSPRINTER_PRINTER_H

typedef struct escpos_config {
    // See ESCPOS_CHUNK_OVERLAP in constants.h
    // Default value: ESCPOS_CHUNK_OVERLAP
    int chunk_overlap;
} escpos_config;

typedef struct escpos_printer {
    int sockfd;
    escpos_config config;
} escpos_printer;

extern escpos_printer *escpos_printer_network(const char * const addr, const short port);
extern int escpos_printer_config(escpos_printer *printer, const escpos_config * const config);
extern void escpos_printer_destroy(escpos_printer *printer);

extern int escpos_printer_raw(escpos_printer *printer, const char * const message, const int len);
extern int escpos_printer_cut(escpos_printer *printer);
extern int escpos_printer_feed(escpos_printer *printer, const int lines);
extern int escpos_printer_image(escpos_printer *printer, const char * const image_path);

#endif
