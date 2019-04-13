// client.h - Interface for client windows
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_CLIENT_H__
#define __TCWM_CLIENT_H__


#include "tcwm.h"
#include "list.h"


// Store information about a client window
struct client {
	xcb_window_t window;     // Actual window
	xcb_window_t frame;      // Frame window
	uint16_t x, y;           // Window position (top left corner)
	uint16_t width, height;  // Window dimensions (pixels)
	uint16_t border_width;   // Window border width
	struct list n_wsclients; // Node in list of clients in workspace
};


// Manage a window
void client_manage(struct tcwm *tcwm, xcb_window_t window, bool older_than_wm);

// Unmanage a window
void client_unmanage(struct tcwm *tcwm, xcb_window_t window);

// Handle configure request
void client_configure(struct tcwm *tcwm, xcb_configure_request_event_t *ev);


#endif // __TCWM_CLIENT_H__
