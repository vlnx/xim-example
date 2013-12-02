#include <stdio.h>
#include <stdlib.h>
#include <X11/Xlib.h>
#include <X11/Xutil.h>
#include <X11/Xos.h>
#include <X11/Xatom.h>
#include <X11/keysym.h>

Display *dis;
Window win;
XEvent event;

XIM xim;
XIC xic;

int main() {

	dis = XOpenDisplay(NULL);

	win = XCreateSimpleWindow(dis, RootWindow(dis, 0), 1, 1, 500, 500, 0, BlackPixel (dis, 0), BlackPixel(dis, 0));


    /* char *l = XSetLocaleModifiers(0); */
    /* printf("%s\n", l); */
    xim = XOpenIM(dis, NULL, NULL, NULL);
    /* printf("%s\n", XLocaleOfIM(xim)); */
	xic = XCreateIC(xim,
                    XNInputStyle, XIMPreeditNone | XIMStatusNone,
                    XNClientWindow, win,
                    XNFocusWindow, win,
                    NULL);

	XSelectInput(dis, win, FocusChangeMask | KeyPressMask);
	XMapWindow(dis, win);

	while (1) {
        XNextEvent(dis, &event);
		switch  (event.type) {
            case FocusIn:
                XSetICFocus(xic);
                break;
            case FocusOut:
                XUnsetICFocus(xic);
                break;
	 		case KeyPress:
                {
                char buf[100];
                KeySym keySym = 0;

                if (xic) {
                    Status status;
                    int len = Xutf8LookupString(xic, &event.xkey, buf,
                                                sizeof(buf), &keySym,
                                                &status);
                    printf("%d %s %d %d\n", len, buf, keySym, status);
                    /* if (status == XLookupChars || status == XLookupBoth) */
                }

				if (keySym == XK_q) {
		/*Close the program if q is pressed.*/
				    /* fprintf() */
				    /* if (xim) */
				    /*     XCloseIM(xim); */
				    /* if (xic) */
				    /*     XDestroyIC(xic); */
				    /* xim = NULL; */
				    /* xic = NULL; */
                    exit(0);}
                }
                break;
		}
	}
    return 0;
}
