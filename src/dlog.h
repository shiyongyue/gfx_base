/*
 * dlog.h: based on previous ilog, xlog.h etc
 * 2015/03/05/zhan
 */

#ifndef _DLOG_H
#define _DLOG_H

#include <stdio.h>
#include <string.h>
#include <sys/time.h>
#include <time.h>

#ifndef LOG_TAG
#define LOG_TAG "shiyy"
#endif
// basename may change its param. so we have our implementation which
// doesn't change its param.
static inline const char *good_basename(const char *path)
{
	if (path) {
		// linux style path separator
		const char *p = strrchr(path, '/');
		if (p) {
			// eat '/'
			++p;
			if (*p) return p;
		} else {
			return path;
		}
	}

	return "";
}

// Shanghai time, 8 hours ahead of GMT, for development
#define SHANGHAI_TIMEZONE 8

static int log_level = 5;

#define BASE_LOG(level, prefix, fmt, ...) \
	do { \
		if (level > log_level) { \
			break; \
		} \
		struct timeval now_; \
		struct tm tm_; \
		gettimeofday(&now_, NULL); \
		gmtime_r(&now_.tv_sec, &tm_); \
		printf("[%02d:%02d:%02d.%06d] %s%s %s@%d " fmt "\n", \
				(tm_.tm_hour + SHANGHAI_TIMEZONE) % 24, \
				tm_.tm_min, \
				tm_.tm_sec, \
				(int)now_.tv_usec, \
				prefix, \
				LOG_TAG, \
				__func__, \
				__LINE__, \
				##__VA_ARGS__); \
	} while (0)

enum {
	DLOG_ERROR  =  1,
	DLOG_WARNING,
	DLOG_INFO,
	DLOG_DEBUG,
	DLOG_VERBOSE
};

# define PCLog(...)    BASE_LOG(DLOG_VERBOSE, "PCLOG ", __VA_ARGS__)
# define TraceLog(...) BASE_LOG(DLOG_VERBOSE, "TRACE ", __VA_ARGS__)
# define DebugLog(...) BASE_LOG(DLOG_DEBUG,   "DEBUG ", __VA_ARGS__)
# define InfoLog(...)  BASE_LOG(DLOG_INFO,    "INFO  ", __VA_ARGS__)
# define WarnLog(...)  BASE_LOG(DLOG_WARNING, "WARN  ", __VA_ARGS__)
# define ErrLog(...)   BASE_LOG(DLOG_ERROR,   "ERROR ", __VA_ARGS__)

#endif //_DLOG_H
