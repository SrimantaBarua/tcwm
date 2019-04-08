// tcwm.h - Global state for TCWM
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_TCWM_H__
#define __TCWM_TCWM_H__


#include <X11/Xlib.h>


// Store tcwm global state
struct tcwm {
	// ---- Xlib stuff --------
	Display *dpy; // Handle to Xlib display
	Window  root; // Handle to root window
};


#endif // __TCWM_TCWM_H__
