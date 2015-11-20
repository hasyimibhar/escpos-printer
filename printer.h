#ifndef ESCPOSPRINTER_PRINTER_H
#define ESCPOSPRINTER_PRINTER_H

typedef struct escpos_printer {
    int sockfd;
} escpos_printer;

extern escpos_printer *escpos_printer_network(const char * const addr, const short port);
extern void escpos_printer_destroy(escpos_printer *printer);

extern int escpos_printer_raw(escpos_printer *printer, const char * const message, const int len);
extern int escpos_printer_cut(escpos_printer *printer);
extern int escpos_printer_feed(escpos_printer *printer, const unsigned char lines);

#endif
