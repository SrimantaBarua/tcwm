// event.h - Implementation of X event handling
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#include "log.h"
#include "event.h"
#include "client.h"


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


// Handle XCB_CREATE_NOTIFY
static void _handle_create_notify(struct tcwm *tcwm, xcb_create_notify_event_t *ev) {
	info_fmt("XCB_CREATE_NOTIFY: window: %u", ev->window);
}


// Handle XCB_CONFIGURE_REQUEST
static void _handle_configure_request(struct tcwm *tcwm, xcb_configure_request_event_t *ev) {
	info_fmt("XCB_CONFIGURE_REQUEST: window: %u", ev->window);
	client_configure(tcwm, ev);
}


// Handle XCB_MAP_REQUEST
static void _handle_map_request(struct tcwm *tcwm, xcb_map_request_event_t *ev) {
	info_fmt("XCB_MAP_REQUEST: window: %u", ev->window);
	client_manage(tcwm, ev->window);
}


// Handle XCB_UNMAP_NOTIFY
static void _handle_unmap_notify(struct tcwm *tcwm, xcb_unmap_notify_event_t *ev) {
	info_fmt("XCB_UNMAP_NOTIFY: window: %u", ev->window);
	client_unmanage(tcwm, ev->window);
}


// Handle X events
int event_handle(struct tcwm *tcwm, xcb_generic_event_t *ev) {
	switch (ev->response_type) {
	case XCB_CREATE_NOTIFY:
		_handle_create_notify(tcwm, (xcb_create_notify_event_t*) ev);
		break;
	case XCB_CONFIGURE_REQUEST:
		_handle_configure_request(tcwm, (xcb_configure_request_event_t*) ev);
		break;
	case XCB_MAP_REQUEST:
		_handle_map_request(tcwm, (xcb_map_request_event_t*) ev);
		break;
	case XCB_UNMAP_NOTIFY:
		_handle_unmap_notify(tcwm, (xcb_unmap_notify_event_t*) ev);
		break;
	//case XCB_MAP_NOTIFY:
	//case XCB_CONFIGURE_NOTIFY:
	//case XCB_REPARENT_NOTIFY:
	//case XCB_DESTROY_NOTIFY:
		//break;
	default:
		info_fmt("Event: %s (%u)", _evname(ev->response_type), ev->response_type);
	}
	return 0;
}
