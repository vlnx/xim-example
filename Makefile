all: xim.o link

xim.o: xim.c
	gcc -c xim.c -I/usr/X11R6/include

link: xim.o
	gcc xim.o -L/usr/X11R6/lib -lX11 -o xim
