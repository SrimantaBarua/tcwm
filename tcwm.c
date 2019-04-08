// tcwm - Tiny compositing window manager
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#include <X11/Xlib.h>
#include <stdlib.h>

#include "log.h"
#include "tcwm.h"
#include "event.h"


// Static instance of struct tcwm. SHOULD NOT BE ACCESSED except from within _cleanup and
// _init_everything
static struct tcwm *__tcwm = NULL;


// First error handler. This checks whether another window manager is running
static int _xerror_otherwm(Display *dpy, XErrorEvent *ee) {
	info("Another window manager is already running");
	exit(1);
}


// Normal X error handler
static int _xerror(Display *dpy, XErrorEvent *ee) {
	debug_fmt("X Error: %u", ee->error_code);
	return 0;
}


// Cleans up resources we're using
static void _cleanup() {
	if (__tcwm && __tcwm->dpy) {
		XCloseDisplay(__tcwm->dpy);
	}
}


// Initialize global state
static struct tcwm* _init_everything() {
	struct tcwm *tcwm;
	// Allocate global state
	if (!(tcwm = calloc(1, sizeof(struct tcwm)))) {
		die_err("calloc()");
	}
	__tcwm = tcwm;
	// Open display and get root window
	if (!(tcwm->dpy = XOpenDisplay(NULL))) {
		die("Unable to open X display");
	}
	atexit(_cleanup);
	tcwm->root = DefaultRootWindow(tcwm->dpy);
	// Detect whether another window manager is running. If not, subscribe for events
	XSetErrorHandler(_xerror_otherwm);
	XSelectInput(tcwm->dpy, tcwm->root, SubstructureRedirectMask | SubstructureNotifyMask);
	XSync(tcwm->dpy, False);
	// Set actual error handler
	XSetErrorHandler(_xerror);
	return tcwm;
}


int main() {
	struct tcwm *tcwm;
	XEvent ev;
	// Initialize everything
	tcwm = _init_everything();
	// Event loop
	while (1) {
		XNextEvent(tcwm->dpy, &ev);
		event_handle(tcwm, &ev);
	}
	return 0;
}
