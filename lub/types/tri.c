/*
 * tri.c
 */

#include "lub/types.h"

#include "lub/string.h"

tri_t lub_tri_from_string(const char *s) {
	if(lub_string_nocasecmp(s, "true") == 0) {
		return TRI_TRUE;
	} else if(lub_string_nocasecmp(s, "false") == 0) {
		return TRI_FALSE;
	} else {
		return TRI_UNDEFINED;
	}
}

bool_t lub_tri_default(tri_t t, bool_t d)
{
	switch(t) {
	case TRI_TRUE:
		return BOOL_TRUE;
	case TRI_FALSE:
		return BOOL_FALSE;
	case TRI_UNDEFINED:
	default:
		return d;
	}
}
