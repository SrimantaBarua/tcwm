// tcwm - Tiny compositing window manager
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#include <errno.h>
#include <stdlib.h>
#include <xcb/xcb.h>
#include <sys/select.h>

#include "log.h"
#include "tcwm.h"
#include "event.h"


// Static instance of struct tcwm. SHOULD NOT BE ACCESSED except from within _cleanup and
// _init_everything
static struct tcwm *__tcwm = NULL;


// Cleans up resources we're using
static void _cleanup() {
	if (__tcwm && __tcwm->conn) {
		xcb_disconnect(__tcwm->conn);
	}
}


// Initialize global state
static struct tcwm* _init_everything() {
	struct tcwm *tcwm;
	xcb_screen_iterator_t iter;
	int i;
	uint32_t mask;
	uint32_t values[2];
	xcb_window_t root;
	xcb_generic_error_t *err;

	// Allocate global state
	if (!(tcwm = calloc(1, sizeof(struct tcwm)))) {
		die_err("calloc()");
	}
	__tcwm = tcwm;
	// Initialize connection to X server
	if (!(tcwm->conn = xcb_connect(NULL, &tcwm->scrnum)) ||  xcb_connection_has_error(tcwm->conn)) {
		die("Unable to open X display");
	}
	atexit(_cleanup);
	info("Opened X server connection");
	// Find our screen
	iter = xcb_setup_roots_iterator(xcb_get_setup(tcwm->conn));
	for (i = 0; i < tcwm->scrnum; i++) {
		xcb_screen_next(&iter);
	}
	if (!(tcwm->screen = iter.data)) {
		die("Could not get current screen. Exiting");
	}
	root = tcwm->screen->root;
	info_fmt("Acquired current screen | size: %ux%u | Root window: %u",
			tcwm->screen->width_in_pixels, tcwm->screen->height_in_pixels, root);
	// Subscribe for events
	mask = XCB_CW_EVENT_MASK;
	values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
	err = xcb_request_check(tcwm->conn, xcb_change_window_attributes_checked(tcwm->conn, root, mask, values));
	if (err) {
		die_fmt("Cannot get SUBSTRUCTURE_REDIRECT. Error code: %u. "
			"Is another window manager running?", err->error_code);
	}
	// Get fd for X connection
	tcwm->xfd = xcb_get_file_descriptor(tcwm->conn);
	// Initialize hash tables
	tcwm->ht_frame = htable_u32_new();
	tcwm->ht_client = htable_u32_new();
	return tcwm;
}


int main() {
	struct tcwm *tcwm;
	xcb_generic_event_t *ev;
	fd_set in;
	int found;
	// Initialize everything
	tcwm = _init_everything();
	// Event loop
	while (1) {
		// Non-blocking poll for events
		if (!(ev = xcb_poll_for_event(tcwm->conn))) {
			// None found. Block on select()
			FD_ZERO(&in);
			FD_SET(tcwm->xfd, &in);
			while ((found = select(tcwm->xfd + 1, &in, NULL,  NULL, NULL)) < 0) {
				if (errno != EAGAIN) {
					die_err("select()");
				}
			}
			// We found more events. Go to outer loop to handle
			continue;
		}
		event_handle(tcwm, ev);
		free(ev);
	}
	return 0;
}
