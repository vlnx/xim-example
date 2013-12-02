all: XInputContext_test.o link

XInputContext_test.o: XInputContext_test.c
	gcc -c XInputContext_test.c -I/usr/X11R6/include

link: XInputContext_test.o
	gcc XInputContext_test.o -L/usr/X11R6/lib -lX11 -o XInputContext_test
