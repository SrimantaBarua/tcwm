// client.c - Implementation of client window handling
// (C) 2019 Srimanta Barua


#include <stdlib.h>

#include "log.h"
#include "client.h"


#define FRAME_COLOR  0xffffff
#define BORDER_COLOR 0x000000
#define FRAME_HEIGHT 20
#define BORDER_WIDTH 1


// Store information about a client window
struct client {
	xcb_window_t window;    // Actual window
	xcb_window_t frame;     // Frame window
	uint16_t x, y;          // Window position (top left corner)
	uint16_t width, height; // Window dimensions (pixels)
	uint16_t border_width;  // Window border width
};


// Frame a window, and return a client struct
static struct client* _frame(struct tcwm *tcwm, xcb_window_t win) {
	xcb_generic_error_t *err;
	xcb_get_geometry_cookie_t g_cookie;
	xcb_void_cookie_t v_cookies[7];
	xcb_get_geometry_reply_t *geom;
	uint32_t mask, i;
	uint32_t values[2];
	struct client *client;
	// Allocate client
	if (!(client = malloc(sizeof(struct client)))) {
		die_err("malloc()");
	}
	client->window = win;
	// Get window geometry
	g_cookie =  xcb_get_geometry(tcwm->conn, win);
	geom = xcb_get_geometry_reply(tcwm->conn, g_cookie, &err);
	if (err) {
		die_fmt("xcb_get_geometry_reply(): Error code: %u", err->error_code);
	}
	if (!geom) {
		die("xcb_get_geometry_reply()");
	}
	client->x = geom->x;
	client->y = geom->y;
	client->width = geom->width;
	client->height = geom->height;
	client->border_width = geom->border_width;
	free(geom);
	// Create frame
	client->frame = xcb_generate_id(tcwm->conn);
	mask =  XCB_CW_BACK_PIXEL | XCB_CW_BORDER_PIXEL;
	values[0] = FRAME_COLOR;
	values[1] = BORDER_COLOR;
	v_cookies[0] = xcb_create_window_checked(tcwm->conn, XCB_COPY_FROM_PARENT, client->frame,
						tcwm->screen->root, client->x, client->y,
						client->width, client->height, BORDER_WIDTH,
						XCB_WINDOW_CLASS_INPUT_OUTPUT,
						tcwm->screen->root_visual, mask, values);
	// Resize origin window to fit within frame, and set border width to 0
	mask = XCB_CONFIG_WINDOW_HEIGHT | XCB_CONFIG_WINDOW_BORDER_WIDTH;
	values[0] = client->height - FRAME_HEIGHT;
	values[1] = 0;
	v_cookies[1] = xcb_configure_window_checked(tcwm->conn, win, mask, values);
	// Request for events on frame
	mask = XCB_CW_EVENT_MASK;
	values[0] = XCB_EVENT_MASK_SUBSTRUCTURE_REDIRECT | XCB_EVENT_MASK_SUBSTRUCTURE_NOTIFY;
	v_cookies[2] = xcb_change_window_attributes_checked(tcwm->conn, client->frame, mask, values);
	// Add original window to save set
	v_cookies[3] = xcb_change_save_set_checked(tcwm->conn, XCB_SET_MODE_INSERT, win);
	// Reparent window (set frame to be original window's parent)
	v_cookies[4] = xcb_reparent_window_checked(tcwm->conn, win, client->frame, 0, FRAME_HEIGHT);
	// Map frame
	v_cookies[5] =  xcb_map_window_checked(tcwm->conn, client->frame);
	// Map window
	v_cookies[6] =  xcb_map_window_checked(tcwm->conn, win);
	// Get all replies and handle errors
	for (i = 0; i < sizeof(v_cookies) / sizeof(v_cookies[0]); i++) {
		if ((err = xcb_request_check(tcwm->conn, v_cookies[i]))) {
			die_fmt("xcb_request_check(): X error code: %u", err->error_code);
		}
	}
	return client;
}


// Manage a window
void client_manage(struct tcwm *tcwm, xcb_window_t window) {
	struct client *client;
	if (!tcwm) {
		die("NULL tcwm");
	}
	if (htable_u32_contains(tcwm->ht_frame, window)) {
		debug_fmt("Window %u already managed", window);
		return;
	}
	// Frame window and manage it
	client = _frame(tcwm, window);
	if (htable_u32_set(tcwm->ht_frame, window, (void*) (uintptr_t) client->frame) != HTE_OK) {
		die_fmt("htable_u32_set(ht_frame, %u, %u)", window, client->frame);
	}
	if (htable_u32_set(tcwm->ht_client, client->frame, (void*) client) != HTE_OK) {
		die_fmt("htable_u32_set(ht_frame, %u, client)", client->frame);
	}
}


// Unmanage a window
void client_unmanage(struct tcwm *tcwm, xcb_window_t window) {
	xcb_generic_error_t *err;
	struct client *client;
	xcb_window_t frame;
	enum htable_err hterr;
	xcb_void_cookie_t v_cookies[1];
	uint32_t i;

	if (!tcwm) {
		die("NULL tcwm");
	}
	// Get frame. If doesn't exist, this is unmanaged. Return
	frame = (xcb_window_t) (uintptr_t) htable_u32_pop(tcwm->ht_frame, window, &hterr);
	if (hterr != HTE_OK) {
		return;
	}
	// This is managed. Get client
	if (!(client = (struct client*) htable_u32_pop(tcwm->ht_client, frame, NULL))) {
		die_fmt("htable_u32_pop(ht_client, %u)", frame);
	}
	// Free client
	free(client);
	// Destroy frame
	v_cookies[0] = xcb_destroy_window_checked(tcwm->conn, frame);
	// Check errors
	for (i = 0; i < sizeof(v_cookies) / sizeof(v_cookies[0]); i++) {
		if ((err = xcb_request_check(tcwm->conn, v_cookies[i]))) {
			die_fmt("xcb_request_check(): X error code: %u %u", err->error_code, i);
		}
	}
}


// Handle configure request
void client_configure(struct tcwm *tcwm, xcb_configure_request_event_t *ev) {
	xcb_generic_error_t *err;
	uint32_t values[7], mask = 0, i = 0;
	struct client *client;
	xcb_window_t frame;
	enum htable_err hterr;

	if (!tcwm) {
		die("NULL tcwm");
	}
	// Is this a managed window?
	frame = (xcb_window_t) (uintptr_t) htable_u32_get(tcwm->ht_frame, ev->window, &hterr);
	if (hterr == HTE_OK) {
		// This is managed. Get client
		if (!(client = (struct client*) htable_u32_pop(tcwm->ht_client, frame, NULL))) {
			die_fmt("htable_u32_pop(ht_client, %u)", frame);
		}
		if (ev->value_mask & XCB_CONFIG_WINDOW_X) {
			mask |= XCB_CONFIG_WINDOW_X;
			values[i++] = ev->x;
			client->x = ev->x;
		}
		if (ev->value_mask & XCB_CONFIG_WINDOW_Y) {
			mask |= XCB_CONFIG_WINDOW_Y;
			// Modify y to be within limits
			if (ev->y < FRAME_HEIGHT) {
				ev->y = FRAME_HEIGHT;
			}
			values[i++] = ev->y - FRAME_HEIGHT;
			client->y = ev->y;
		}
		if (ev->value_mask & XCB_CONFIG_WINDOW_WIDTH) {
			mask |= XCB_CONFIG_WINDOW_WIDTH;
			values[i++] = ev->width;
			client->width = ev->width;
		}
		if (ev->value_mask & XCB_CONFIG_WINDOW_HEIGHT) {
			mask |= XCB_CONFIG_WINDOW_HEIGHT;
			values[i++] = ev->height + FRAME_HEIGHT;
			client->x = ev->x;
		}
		if (ev->value_mask & XCB_CONFIG_WINDOW_BORDER_WIDTH) {
			mask |= XCB_CONFIG_WINDOW_BORDER_WIDTH;
			values[i++] = ev->border_width;
			client->border_width = ev->border_width;
			ev->border_width = 0;
		}
		err = xcb_request_check(tcwm->conn, xcb_configure_window_checked(tcwm->conn, frame, mask, values));
		if (err) {
			die_fmt("xcb_configure_window_checked(): Error code: %u", err->error_code);
		}
	}
	mask = 0;
	i = 0;
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
