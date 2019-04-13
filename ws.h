// ws.h - Interface for workspaces
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_WS_H__
#define __TCWM_WS_H__


#include "list.h"


// Store information about a workspace
struct ws {
	struct list h_clients;  // List of clients in workspace. Maintained in stacking order
};


#endif // __TCWM_WS_H__
