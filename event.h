// event.h - Handle X events
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_EVENT_H__
#define __TCWM_EVENT_H__


#include "tcwm.h"


// Handle X events
int event_handle(struct tcwm *tcwm, XEvent *ev);


#endif // __TCWM_EVENT_H__
