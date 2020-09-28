#ifdef DEBUG

/*
 * shell_dump.c
 */
#include "private.h"
#include "lub/dump.h"

/*--------------------------------------------------------- */
void clish_shell_dump(clish_shell_t * this)
{
	clish_view_t *v;
	clish_ptype_t *t;
	clish_var_t *var;
	lub_list_node_t *iter;
	lub_bintree_iterator_t iterc;

	lub_dump_printf("shell(%p)\n", this);
	lub_dump_printf("OVERVIEW:\n%s\n", LUB_DUMP_STR(this->overview));
	lub_dump_indent();

	/* iterate the list of views */
	for(iter = lub_list__get_head(this->view_tree);
	    iter; iter = lub_list_node__get_next(iter)) {
		v = (clish_view_t *)lub_list_node__get_data(iter);
		clish_view_dump(v);
	}

	/* iterate the list of types */
	for(iter = lub_list__get_head(this->ptype_tree);
	    iter; iter = lub_list_node__get_next(iter)) {
		t = (clish_ptype_t *)lub_list_node__get_data(iter);
		clish_ptype_dump(t);
	}

	/* iterate the tree of vars */
	var = lub_bintree_findfirst(&this->var_tree);
	for (lub_bintree_iterator_init(&iterc, &this->var_tree, var);
		var; var = lub_bintree_iterator_next(&iterc)) {
		clish_var_dump(var);
	}

	lub_dump_undent();
}

/*--------------------------------------------------------- */

#endif /* DEBUG */
