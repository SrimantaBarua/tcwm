// tcwm.h - Global state for TCWM
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_TCWM_H__
#define __TCWM_TCWM_H__


#include <xcb/xcb.h>

#include "ws.h"
#include "htable.h"


#define FRAME_COLOR  0xffffff
#define BORDER_COLOR 0x000000
#define FRAME_HEIGHT 20
#define BORDER_WIDTH 1

#define WS_HEIGHT 2
#define WS_WIDTH  3


// Store tcwm global state
struct tcwm {
	// ---- XCB stuff --------
	xcb_connection_t  *conn;      // Connection to X server
	xcb_screen_t      *screen;    // Structure for default screen
	int               scrnum;     // Default screen number
	int               xfd;        // FD for X connection
	// ---- Workspaces --------
	struct ws         *ws;        // Pointer to workspace[0][0] (top left in grid)
	unsigned          ws_width;   // Number of horizontal workspace
	unsigned          ws_height;  // Number of vertical workspace
	// ---- Clients --------
	uint32_t          frame_height; // Height of frame window
	uint32_t          frame_color;  // Color of frame window background
	uint32_t          border_color; // Color of frame window border
	uint32_t          border_width; // Width of frame window border
	struct htable_u32 *ht_frame;  // Map child window_id to frame window_id
	struct htable_u32 *ht_client; // Map frame window_id to client structure
};


#endif // __TCWM_TCWM_H__
