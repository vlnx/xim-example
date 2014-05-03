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


void utf8print(unsigned int cp) {
    if (cp < 0x80) {
        printf("%c", cp);
    } else if (cp < 0x800) {
        printf("%c%c", 0xC0 + cp / 0x40, 0x80 + cp % 0x40);
    }
    // you should implement the three- and four-byte cases, too.
}

// rxvt.h
/* #define KBUFSZ 512 // size of keyboard mapping buffer */
/* wchar_t wkbuf[KBUFSZ + 1]; */
/* wkbuf, KBUFSZ, &keySym, &status); */

void key_press(XKeyEvent ev) {

    /* ev.serial = 0; */

    /* printf("%d | %d | %d;\n", */
    /*         ev.serial, */
    /*         ev.state, */
    /*         ev.keycode */
    /*         ); */


    if (xic == NULL) exit(1);
    // Retrieve unicode string
    int len;
    KeySym keySym;
    Status status;
    unsigned int wkbuf[513];
    len = XwcLookupString(xic, &ev,
                                wkbuf, 512, &keySym, &status);
    // no string, some funcion key pressed
    printf("%d ;;", status);
    /* if (status == XLookupKeySym) return; */

    printf("%d ", keySym);
    printf("%d ", (unsigned int)wkbuf[0]);
    utf8print((unsigned int)wkbuf[0]);
    fputc(';', stdout);
    fputc('\n', stdout);
    fputc('\n', stdout);
}

int main() {

/* May be what's missing in cffi */
    printf("%s\n", setlocale(LC_ALL, NULL));
    printf("%s\n", setlocale(LC_ALL, ""));
    printf("%s\n", setlocale(LC_ALL, NULL));

    if (setlocale(LC_ALL, "") == NULL) exit(1);
    if (XSetLocaleModifiers("") == NULL) exit(1);

    dis = XOpenDisplay(NULL);
    win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500, 0,
            BlackPixel (dis, 0), BlackPixel(dis, 0));
    XMapWindow(dis, win);

    xim = XOpenIM(dis, NULL, 0, 0);
    xic = XCreateIC(xim,
                    "inputStyle", 1032,
                    /* "inputStyle", 0x0008L | 0x0400L, */
                    /* "inputStyle", XIMPreeditNothing | XIMStatusNothing, */
                    /* XNInputStyle, XIMPreeditNothing | XIMStatusNothing, */
                    "clientWindow", win,
                    /* XNClientWindow, win, */
                    "focusWindow", win,
                    /* XNFocusWindow, win, */
                    NULL);
    if (xic == NULL) exit(1);

    XSelectInput(dis, win, KeyPressMask);

    while (1) {
        XEvent event;
        XNextEvent(dis, &event);
        /* printf("%d::\n", event.type); */
        /* printf("%d::\n", event.xany); */
        /* printf("%d::\n", event.xany.window); */

        /* if (XFilterEvent(&event, None)) continue; */
        /* XFilterEvent(&event, 0); */
        /* XFilterEvent(&event, 0); */

        switch (event.type) {
             case KeyPress:
                /* if (XFilterEvent(&event, 0)) continue; */
                if (XFilterEvent((XEvent *)&event.xkey, 0)) continue;
                key_press(event.xkey);
                break;
        }
    }
    return 0;
}
