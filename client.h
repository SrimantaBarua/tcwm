// client.h - Interface for client windows
// (C) 2019 Srimanta Barua


#ifndef __TCWM_CLIENT_H__
#define __TCWM_CLIENT_H__


#include "tcwm.h"


// Manage a window
void client_manage(struct tcwm *tcwm, xcb_window_t window);

// Unmanage a window
void client_unmanage(struct tcwm *tcwm, xcb_window_t window);

// Handle configure request
void client_configure(struct tcwm *tcwm, xcb_configure_request_event_t *ev);


#endif // __TCWM_CLIENT_H__
