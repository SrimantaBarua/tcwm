// tcwm.h - Global state for TCWM
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_TCWM_H__
#define __TCWM_TCWM_H__


#include <xcb/xcb.h>


// Store tcwm global state
struct tcwm {
	// ---- XCB stuff --------
	xcb_connection_t *conn;   // Connection to X server
	xcb_screen_t     *screen; // Structure for default screen
	int              scrnum;  // Default screen number
	int              xfd;     // FD for X connection
};


#endif // __TCWM_TCWM_H__
