CC = gcc
CFLAGS = -Wall -Werror

libescposprinter.a: printer.o error.o serial.o dist
	ar r dist/libescposprinter.a obj/printer.o obj/error.o obj/serial.o
	cp {escpos_printer.h,printer.h,constants.h,error.h,serial.h} dist/

printer.o: printer.c printer.h error_private.h constants.h serial.h obj
	$(CC) -c $(CFLAGS) -o obj/printer.o printer.c

error.o: error.c error.h obj
	$(CC) -c $(CFLAGS) -o obj/error.o error.c

serial.o: serial.c serial.h obj
	$(CC) -c $(CFLAGS) -o obj/serial.o serial.c

clean:
	rm -rf dist obj

dist:
	mkdir -p dist

obj:
	mkdir -p obj
