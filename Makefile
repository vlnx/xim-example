all:
	gcc -c xim.c
	gcc xim.o -lX11 -o xim

clean:
	rm xim.o
