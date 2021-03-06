/*
 * view.h
 */
 /**
\ingroup clish
\defgroup clish_view view
@{

\brief This class is a container of commands.

A particular CLI session may contain a number of different views. Each 
view may contain its own specific commands as well as those available at
a global scope.

*/
#ifndef _clish_view_h
#define _clish_view_h

typedef struct clish_view_s clish_view_t;

typedef enum {
	CLISH_RESTORE_NONE,
	CLISH_RESTORE_DEPTH,
	CLISH_RESTORE_VIEW,
	CLISH_RESTORE_MAX
} clish_view_restore_e;

#include "lub/list.h"
#include "clish/command.h"
#include "clish/nspace.h"
#include "clish/var.h"

clish_view_t *clish_view_new(const char *name);
int clish_view_compare(const void *clientnode, const void *clientkey);
void clish_view_delete(void *instance);
clish_command_t *clish_view_new_command(clish_view_t * instance,
	const char *name, const char *text);
bool_t clish_view_insert_command(clish_view_t * instance,
	clish_command_t *cmd);
bool_t clish_view_remove_command(clish_view_t * instance,
	clish_command_t *cmd);
clish_command_t *clish_view_find_command(clish_view_t * instance,
	const char *name, bool_t inherit);
const clish_command_t *clish_view_find_next_completion(clish_view_t * instance,
	const char *iter_cmd, const char *line,
	clish_nspace_visibility_e field, bool_t inherit);
clish_command_t *clish_view_resolve_command(clish_view_t * instance,
	const char *line, bool_t inherit);
clish_command_t *clish_view_resolve_prefix(clish_view_t * instance,
	const char *line, bool_t inherit);
void clish_view_dump(clish_view_t * instance);
void clish_view_insert_nspace(clish_view_t * instance, clish_nspace_t * nspace);
void clish_view_clean_proxy(clish_view_t * instance);
int clish_view_insert_hotkey(const clish_view_t *instance, const char *key, const char *cmd);
const char *clish_view_find_hotkey(const clish_view_t *instance, int code);

_CLISH_GET(view, lub_list_t *, nspaces);
_CLISH_GET_STR(view, name);
_CLISH_SET_STR_ONCE(view, prompt);
_CLISH_GET_STR(view, prompt);
_CLISH_SET_STR(view, access);
_CLISH_GET_STR(view, access);
_CLISH_SET(view, unsigned int, depth);
_CLISH_GET(view, unsigned int, depth);
_CLISH_SET(view, clish_view_restore_e, restore);
_CLISH_GET(view, clish_view_restore_e, restore);

lub_bintree_t * clish_view__get_tree(clish_view_t *instance);

const char *clish_view_restore__get_name(clish_view_restore_e
	restore);
clish_view_restore_e clish_view_restore_resolve(const char
	*restore_name);

#endif				/* _clish_view_h */
/** @} clish_view */
