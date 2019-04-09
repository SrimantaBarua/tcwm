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


// Handle XCB_CREATE_NOTIFY
static void _handle_create_notify(struct tcwm *tcwm, xcb_create_notify_event_t *ev) {
	info_fmt("XCB_CREATE_NOTIFY: window: %u", ev->window);
}


// Handle XCB_CONFIGURE_REQUEST
static void _handle_configure_request(struct tcwm *tcwm, xcb_configure_request_event_t *ev) {
	xcb_generic_error_t *err;
	uint32_t values[7], mask = 0, i = 0;

	info_fmt("XCB_CONFIGURE_REQUEST: window: %u", ev->window);
	if (ev->value_mask & XCB_CONFIG_WINDOW_X) {
		mask |= XCB_CONFIG_WINDOW_X;
		values[i++] = ev->x;
	}
	if (ev->value_mask & XCB_CONFIG_WINDOW_Y) {
		mask |= XCB_CONFIG_WINDOW_Y;
		values[i++] = ev->y;
	}
	if (ev->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
		mask |= XCB_CONFIG_WINDOW_WIDTH;
		values[i++] = ev->width;
	}
	if (ev->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
		mask |= XCB_CONFIG_WINDOW_HEIGHT;
		values[i++] = ev->height;
	}
	if (ev->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) {
		mask |= XCB_CONFIG_WINDOW_BORDER_WIDTH;
		values[i++] = ev->border_width;
	}
	if (ev->value_mask & XCB_CONFIG_WINDOW_SIBLING) {
		mask |= XCB_CONFIG_WINDOW_SIBLING;
		values[i++] = ev->sibling;
	}
	if (ev->value_mask & XCB_CONFIG_WINDOW_STACK_MODE) {
		mask |= XCB_CONFIG_WINDOW_STACK_MODE;
		values[i++] = ev->stack_mode;
	}
	err = xcb_request_check(tcwm->conn, xcb_configure_window_checked(tcwm->conn, ev->window, mask, values));
	if (err) {
		die_fmt("xcb_configure_window_checked(): Error code: %u", err->error_code);
	}
}


// Handle XCB_MAP_REQUEST
static void _handle_map_request(struct tcwm *tcwm, xcb_map_request_event_t *ev) {
	xcb_generic_error_t *err;

	info_fmt("XCB_MAP_REQUEST: window: %u", ev->window);
	err = xcb_request_check(tcwm->conn, xcb_map_window_checked(tcwm->conn, ev->window));
	if (err) {
		die_fmt("xcb_map_window_checked(): Error code: %u", err->error_code);
	}
}


// Handle XCB_UNMAP_NOTIFY
static void _handle_unmap_notify(struct tcwm *tcwm, xcb_unmap_notify_event_t *ev) {
	info_fmt("XCB_UNMAP_NOTIFY: window: %u", ev->window);
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
	default:
		info_fmt("Event: %s (%u)", _evname(ev->response_type), ev->response_type);
	}
	return 0;
}
