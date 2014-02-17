#include <stdio.h>
#include <stdlib.h>
#include <locale.h>
#include <X11/Xlib.h>
#include <X11/Xlocale.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

Display *dis;
Window win;
XEvent event;

XIM xim;
XIC xic;

// rxvt.h
#define KBUFSZ 512 // size of keyboard mapping buffer

void utf8print(unsigned int cp) {
    if (cp < 0x80) {
        printf("%c", cp);
    } else if (cp < 0x800) {
        printf("%c%c", 0xC0 + cp / 0x40, 0x80 + cp % 0x40);
    }
    // you should implement the three- and four-byte cases, too.
}

void key_press(XKeyEvent ev) {
    int len;
    KeySym keySym;
    wchar_t wkbuf[KBUFSZ + 1];
    char kbuf[KBUFSZ];
    kbuf[0] = 0;

    if (xic == NULL) exit(1);

    Status status;
    // Retrieve unicode string
    len = XwcLookupString(xic, &ev,
                                wkbuf, KBUFSZ, &keySym, &status);
    // no string, some funcion key pressed
    if (status == XLookupKeySym) return;

    printf("%d ", (unsigned int)wkbuf[0]);
    utf8print((unsigned int)wkbuf[0]);
    fputc('\n', stdout);
}

int main() {

    if (setlocale(LC_ALL, "") == NULL) exit(1);
    if (XSetLocaleModifiers("") == NULL) exit(1);

	dis = XOpenDisplay(NULL);
	win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500, 0,
	        BlackPixel (dis, 0), BlackPixel(dis, 0));

    xim = XOpenIM(dis, NULL, 0, 0);
	xic = XCreateIC(xim,
                    XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                    XNClientWindow, win,
                    XNFocusWindow, win,
                    NULL);
    if (xic == NULL) exit(1);

    long im_event_mask;
    XGetICValues(xic, XNFilterEvents, &im_event_mask, NULL);
	XSelectInput(dis, win, FocusChangeMask | KeyPressMask | im_event_mask);
    XSetICFocus(xic);
	XMapWindow(dis, win);

	while (1) {
	    XEvent event;
        XNextEvent(dis, &event);
        if (XFilterEvent(&event, None)) continue;
		switch  (event.type) {
            case FocusIn:
                XSetICFocus(xic);
                break;
            case FocusOut:
                XUnsetICFocus(xic);
                break;
	 		case KeyPress:
                key_press(event.xkey);
                break;
		}
	}
    return 0;
}
