CC = gcc
CFLAGS = -Wall -Werror

libescposprinter.a: printer.o error.o dist
	ar r dist/libescposprinter.a obj/printer.o obj/error.o
	cp {escpos_printer.h,printer.h,constants.h,error.h} dist/

printer.o: printer.c printer.h error_private.h constants.h obj
	$(CC) -c $(CFLAGS) -o obj/printer.o printer.c

error.o: error.c error.h obj
	$(CC) -c $(CFLAGS) -o obj/error.o error.c

clean:
	rm -rf dist obj

dist:
	mkdir -p dist

obj:
	mkdir -p obj
