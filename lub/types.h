/*
 * types.h
 */
/**
\ingroup  lub
\defgroup lub_types types

\brief This provides some primative types not found in ANSI-C.
@{
*/

#ifndef _lub_types_h
#define _lub_types_h

/**
 * A boolean type for ANSI-C
 */
typedef enum {
	BOOL_FALSE,
	BOOL_TRUE
} bool_t;

/**
 * A tri-state boolean. The possible values are
 * TRI_FALSE, TRI_TRUE, TRI_UNDEFINED.
 */
typedef enum {
       TRI_UNDEFINED = -1,
       TRI_FALSE = 0,
       TRI_TRUE = 1
} tri_t;

/**
 * Converts a string to a tri
 *
 * "true" yields TRI_TRUE.
 * "false" yields TRI_FALSE.
 * Anything else is TRI_FALSE.
 */
tri_t lub_tri_from_string(const char *s);

/**
 * Reduce a tri to bool using a default value
 *
 * Will return D if T == TRI_UNDEFINED.
 *
 * Returns boolean value of T otherwise.
 */
bool_t lub_tri_default(tri_t t, bool_t d);


/** @} */
#endif				/* _lub_types_h */
