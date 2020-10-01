
#include <lub/error.h>

#include <stdarg.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#ifdef DEBUG
#define DEFAULT_LOGLEVEL LUB_LOGLEVEL_INFO
#else
#define DEFAULT_LOGLEVEL LUB_LOGLEVEL_NOTICE
#endif

static lub_loglevel_e loglevel = DEFAULT_LOGLEVEL;

void lub_set_loglevel(lub_loglevel_e level) {
	if(level >= 0 && level < _LUB_NUM_LOGLEVEL) {
		loglevel = level;
	}
}

void _lub_message(lub_loglevel_e level, const char *file, int line, const char *function, const char *fmt, ...) {
	va_list args;
	const char *prefix;
	switch(level) {
	case LUB_LOGLEVEL_FATAL:
		prefix = "F";
		break;
	case LUB_LOGLEVEL_ERROR:
		prefix = "E";
		break;
	case LUB_LOGLEVEL_WARNING:
		prefix = "W";
		break;
	case LUB_LOGLEVEL_NOTICE:
		prefix = "N";
		break;
	case LUB_LOGLEVEL_INFO:
		prefix = "I";
		break;
	case LUB_LOGLEVEL_DEBUG:
		prefix = "D";
		break;
	case LUB_LOGLEVEL_TRACE:
		prefix = "T";
		break;
	default:
		prefix = "?";
		break;
	}
#ifdef DEBUG
	fprintf(stderr, "[%s] %s: ", prefix, function);
#else
	fprintf(stderr, "[%s] ", prefix);
#endif
	va_start(args, fmt);
	vfprintf(stderr, fmt, args);
	va_end(args);
#ifdef DEBUG
	fprintf(stderr, " (at %s:%d)\n", file, line);
#else
	fprintf(stderr, "\n");
#endif
	if(level == LUB_LOGLEVEL_FATAL) {
		exit(1);
	}
}
