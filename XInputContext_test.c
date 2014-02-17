#include <stdio.h>
#include <stdlib.h>
#include <wchar.h>
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

long im_event_mask;

// rxvt.h
#define KBUFSZ 512 // size of keyboard mapping buffer

void utf8print(unsigned int cp)
{
    if (cp < 0x80)
    {
            printf("%c", cp);
        }
    else if (cp < 0x800)
    {
            printf("%c%c", 0xC0 + cp / 0x40, 0x80 + cp % 0x40);
        }
    // you should implement the three- and four-byte cases, too.
    //
}

void
key_press(XKeyEvent ev) {

    int len;
    /* printf("Translate invoked\n"); */

    KeySym keySym;
    /* static wchar_t buffer[16]; */
    wchar_t wkbuf[KBUFSZ + 1];

    char kbuf[KBUFSZ];
    kbuf[0] = 0;

    if (xic) {
        Status status;
        // Retrieve unicode string
        len = XwcLookupString(xic, &ev,
                                 wkbuf, KBUFSZ, &keySym, &status);
        /* printf("%d",len); */
        /* switch (status) { */
        /*     case XBufferOverflow: printf("XBufferOverflow\n"); break; */
        /*     case XLookupNone: printf("XLookupNone\n"); break; */
        /*     case XLookupChars: printf("XLookupChars\n"); break; */
        /*     case XLookupKeySym: printf("XLookupKeySym\n"); break; */
        /*     case XLookupBoth: printf("XLookupBoth\n"); break; */
        /* } */

        // no string, some funcion key pressed
        if (status == XLookupKeySym) return;

        /* if (status == XLookupChars || status == XLookupBoth) { */
        /*     wkbuf[len] = 0; */
        /*     len = wcstombs( (char *)kbuf, wkbuf, KBUFSZ); */
        /*     if (len < 0) len = 0; */
        /* } else { len = 0; } */

        printf("%d ", (unsigned int)wkbuf[0]);
        utf8print((unsigned int)wkbuf[0]);
        fputc('\n', stdout);

        /* wprintf(L"Buf is %lc (%x)\n", wkbuf, wkbuf); */
        /* printf("%d\n", (unsigned int)wkbuf[1]); */
        /* printf("%c %c\n", keySym, kbuf); */
        /* wprintf(L"char is %lc (%x)\n", keySym, keySym); */
        /* printf("%d %s\n", keySym, kbuf); */
        /* printf("Status %d\n", status); */
    } else { printf("XIC is null"); }
}

/* http://www.sbin.org/doc/Xlib/chapt_11.html */
int main() {
    /* wchar_t myChar1 = L'Ω'; */
    /* setlocale(LC_CTYPE, ""); */
    if (setlocale(LC_ALL, "") == NULL) exit(1);
    if (XSetLocaleModifiers("") == NULL) exit(1);
    /* wprintf(L"char is %lc (%x)\n", myChar1, myChar1); */

	dis = XOpenDisplay(NULL);
	win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500, 0,
	        BlackPixel (dis, 0), BlackPixel(dis, 0));

    /* xim = XOpenIM(dis, NULL, NULL, NULL); */
    xim = XOpenIM(dis, NULL, 0, 0);
	xic = XCreateIC(xim,
                    /* XNInputStyle, XIMPreeditNone | XIMStatusNone, */
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

/* char buf[100]; */
/* KeySym keySym = 0; */

/* if (xic) { */
/*     Status status; */
/*     int len = Xutf8LookupString(xic, &event.xkey, buf, */
/*                                 sizeof(buf), &keySym, */
/*                                 &status); */
/*     printf("%d %s %d %d\n", len, buf, keySym, status); */
/* } */

/* if (keySym == XK_q) { */
/* if (xim) */
/*     XCloseIM(xim); */
/* if (xic) */
/*     XDestroyIC(xic); */
/* xim = NULL; */
/* xic = NULL; */
/* exit(0);} */
