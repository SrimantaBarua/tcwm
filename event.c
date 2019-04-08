// event.h - Implementation of X event handling
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#include "log.h"
#include "event.h"


// Return string representation of X event
static const char *_evname(int ev) {
	static const char *_names[] = {
		"",
		"",
		"KeyPress",
		"KeyRelease",
		"ButtonPress",
		"ButtonRelease",
		"MotionNotify",
		"EnterNotify",
		"LeaveNotify",
		"FocusIn",
		"FocusOut",
		"KeymapNotify",
		"Expose",
		"GraphicsExpose",
		"NoExpose",
		"VisibilityNotify",
		"CreateNotify",
		"DestroyNotify",
		"UnmapNotify",
		"MapNotify",
		"MapRequest",
		"ReparentNotify",
		"ConfigureNotify",
		"ConfigureRequest",
		"GravityNotify",
		"ResizeRequest",
		"CirculateNotify",
		"CirculateRequest",
		"PropertyNotify",
		"SelectionClear",
		"SelectionRequest",
		"SelectionNotify",
		"ColormapNotify",
		"ClientMessage",
		"MappingNotify"
	};
	if (ev >= 34) {
		return "";
	}
	return _names[ev];
}


// Handle CreateNotify event
static void _handle_create_notify(struct tcwm *tcwm, XCreateWindowEvent *ev) {
	info_fmt("CreateNotify: window: %lu", ev->window);
}


// Handle ConfigureRequest event
static void _handle_configure_request(struct tcwm *tcwm, XConfigureRequestEvent *ev) {
	XWindowChanges changes;
	changes.x = ev->x;
	changes.y = ev->y;
	changes.width = ev->width;
	changes.height = ev->height;
	changes.border_width = ev->border_width;
	changes.sibling = ev->above;
	changes.stack_mode = ev->detail;
	XConfigureWindow(tcwm->dpy, ev->window, ev->value_mask, &changes);
	XSync(tcwm->dpy, False);
}


// Handle MapRequest event
static void _handle_map_request(struct tcwm *tcwm, XMapRequestEvent *ev) {
	info_fmt("MapRequest: window: %lu", ev->window);
	XMapWindow(tcwm->dpy, ev->window);
}


// Handle X events
int event_handle(struct tcwm *tcwm, XEvent *ev) {
	switch (ev->type) {
	case CreateNotify:
		_handle_create_notify(tcwm, &ev->xcreatewindow);
		break;
	case ConfigureRequest:
		_handle_configure_request(tcwm, &ev->xconfigurerequest);
		break;
	case MapRequest:
		_handle_map_request(tcwm, &ev->xmaprequest);
		break;
	default:
		info_fmt("Event: %s (%u)", _evname(ev->type), ev->type);
	}
	return 0;
}
