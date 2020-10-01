/*
 * error.h
 *
 */
#ifndef _lub_error_h
#define _lub_error_h

#include <stdio.h>

#include "lub/c_decl.h"
#include "lub/types.h"

typedef enum {
	      LUB_LOGLEVEL_FATAL = 0,
	      LUB_LOGLEVEL_ERROR,
	      LUB_LOGLEVEL_WARNING,
	      LUB_LOGLEVEL_NOTICE,
	      LUB_LOGLEVEL_INFO,
	      LUB_LOGLEVEL_DEBUG,
	      LUB_LOGLEVEL_TRACE,
	      _LUB_NUM_LOGLEVEL
} lub_loglevel_e;

void lub_set_loglevel(lub_loglevel_e level);

void _lub_message(lub_loglevel_e level, const char *file, int line,
		  const char *function, const char *fmt, ...);

#define lub_fatal(fmt, ...)					\
	_lub_message(LUB_LOGLEVEL_FATAL, __FILE__, __LINE__,	\
		     __FUNCTION__, fmt, ##__VA_ARGS__)
#define lub_error(fmt, ...)					\
	_lub_message(LUB_LOGLEVEL_ERROR, __FILE__, __LINE__,	\
		     __FUNCTION__, fmt, ##__VA_ARGS__)
#define lub_warning(fmt, ...)					\
	_lub_message(LUB_LOGLEVEL_WARNING, __FILE__, __LINE__,	\
		     __FUNCTION__, fmt, ##__VA_ARGS__)
#define lub_notice(fmt, ...)				      \
	_lub_message(LUB_LOGLEVEL_NOTICE, __FILE__, __LINE__, \
		     __FUNCTION__, fmt, ##__VA_ARGS__)
#define lub_info(fmt, ...) \
	_lub_message(LUB_LOGLEVEL_INFO, __FILE__, __LINE__,	\
		     __FUNCTION__, fmt, ##__VA_ARGS__)

#ifdef DEBUG
#define lub_debug(fmt, ...)				     \
	_lub_message(LUB_LOGLEVEL_DEBUG, __FILE__, __LINE__, \
		     __FUNCTION__, fmt, ##__VA_ARGS__)
#define lub_trace(fmt, ...)				     \
	_lub_message(LUB_LOGLEVEL_TRACE, __FILE__, __LINE__, \
		     __FUNCTION__, fmt, ##__VA_ARGS__)
#else
#define lub_debug(...)
#define lub_trace(...)
#endif

#endif				/* _lub_error_h */
