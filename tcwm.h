// tcwm.h - Global state for TCWM
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_TCWM_H__
#define __TCWM_TCWM_H__


#include <xcb/xcb.h>

#include "htable.h"


// Store tcwm global state
struct tcwm {
	// ---- XCB stuff --------
	xcb_connection_t  *conn;      // Connection to X server
	xcb_screen_t      *screen;    // Structure for default screen
	int               scrnum;     // Default screen number
	int               xfd;        // FD for X connection
	// ---- Clients --------
	struct htable_u32 *ht_frame;  // Map child window_id to frame window_id
	struct htable_u32 *ht_client; // Map frame window_id to client structure
};


#endif // __TCWM_TCWM_H__
