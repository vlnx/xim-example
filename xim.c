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

void key_press(XKeyEvent ev) {
    if (xic == NULL) exit(1);
    // Retrieve unicode string
    int len;
    KeySym keySym;
    Status status;
    unsigned int wkbuf[513];
    len = XwcLookupString(xic, &ev,
                                wkbuf, 512, &keySym, &status);
    // no string, some funcion key pressed
    if (status == XLookupKeySym) return;

    utf8print((unsigned int)wkbuf[0]);
    printf("::%d::%d\n"
           ,(unsigned int)wkbuf[0]
           ,keySym);
}

int main() {
    if (setlocale(LC_ALL, "") == NULL) exit(1);
    if (XSetLocaleModifiers("") == NULL) exit(1);

    dis = XOpenDisplay(NULL);
    win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500, 0,
            BlackPixel (dis, 0), BlackPixel(dis, 0));
    XMapWindow(dis, win);

    xim = XOpenIM(dis, NULL, 0, 0);
    xic = XCreateIC(xim,
                    XNInputStyle, XIMPreeditNothing | XIMStatusNothing,
                    XNClientWindow, win,
                    XNFocusWindow, win,
                    NULL);
    if (xic == NULL) exit(1);

    XSelectInput(dis, win, KeyPressMask);

    while (1) {
        XEvent event;
        XNextEvent(dis, &event);
        switch (event.type) {
             case KeyPress:
                if (XFilterEvent(&event, 0)) continue;
                key_press(event.xkey);
                break;
        }
    }
    return 0;
}
