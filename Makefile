SRCS = src/main.c src/helpers.c
CFLAGS = -Iinclude

compile:
	gcc $(SRCS) $(CFLAGS) -o uart.o
