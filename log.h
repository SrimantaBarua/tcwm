// log.h - Interface for logging
// (C) 2019 Srimanta Barua <srimanta.barua1@gmail.com>


#ifndef __TCWM_LOG_H__
#define __TCWM_LOG_H__


#include <stdio.h>
#include <errno.h>
#include <stdlib.h>
#include <string.h>


// -------- Debug messages ----------------

#define debug(msg) \
	fprintf(stderr, "[DEBUG] %s:%s:%d: " msg "\n", __FILE__, __func__, __LINE__)

#define debug_fmt(fmt, ...) \
	fprintf(stderr, "[DEBUG] %s:%s:%d: " fmt "\n", __FILE__, __func__, __LINE__, __VA_ARGS__)


// -------- Info messages ----------------

#define info(msg) \
	fprintf(stderr, "[INFO] %s:%s:%d: " msg "\n", __FILE__, __func__, __LINE__)

#define info_fmt(fmt, ...) \
	fprintf(stderr, "[INFO] %s:%s:%d: " fmt "\n", __FILE__, __func__, __LINE__, __VA_ARGS__)


// -------- Warning messages ----------------

#define warn(msg) \
	fprintf(stderr, "[WARN] %s:%s:%d: " msg "\n", __FILE__, __func__, __LINE__)

#define warn_fmt(fmt, ...) \
	fprintf(stderr, "[WARN] %s:%s:%d: " fmt "\n", __FILE__, __func__, __LINE__, __VA_ARGS__)

#define warn_err(msg) \
	fprintf(stderr, "[WARN] %s:%s:%d: " msg ": %s\n", __FILE__, __func__, __LINE__, strerror(errno))


// -------- Error messages ----------------

#define die(msg) \
	do { \
	fprintf(stderr, "[ERROR] %s:%s:%d: " msg "\n", __FILE__, __func__, __LINE__); \
		exit(1); \
	} while (0)

#define die_fmt(fmt, ...) \
	do { \
		fprintf(stderr, "[ERROR] %s:%s:%d: " fmt "\n", __FILE__, __func__, __LINE__, __VA_ARGS__); \
		exit(1); \
	} while (0)

#define die_err(msg) \
	do { \
		fprintf(stderr, "[ERROR] %s:%s:%d: " msg ":%s\n", __FILE__, __func__, __LINE__, strerror(errno)); \
		exit(1); \
	} while (0)


#endif //  __TCWM_LOG_H__
