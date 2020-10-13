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


/** @} */
#endif				/* _lub_types_h */
