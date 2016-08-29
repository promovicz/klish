/*
 * view.h
 */
#include "clish/view.h"
#include "lub/bintree.h"
#include "lub/list.h"
#include "clish/hotkey.h"

/*---------------------------------------------------------
 * PRIVATE TYPES
 *--------------------------------------------------------- */
struct clish_view_s {
	lub_bintree_t tree;
	lub_bintree_node_t bt_node;
	char *name;
	char *prompt;
	char *access;
	clish_action_t *action;
	lub_list_t *nspaces;
	clish_hotkeyv_t *hotkeys;
	unsigned int depth;
	clish_view_restore_e restore;
};
