/*
 * shell_plugin.c
 */
#include "private.h"
#include <assert.h>
#include <string.h>

#include "lub/string.h"
#include "lub/list.h"
#include "lub/bintree.h"
#include "clish/plugin.h"
#include "clish/view.h"

/*----------------------------------------------------------------------- */
/* For all plugins:
 *  * dlopen(plugin)
 *  * dlsym(initialize function)
 *  * exec init functions to get all plugin syms
 */
int clish_shell_load_plugins(clish_shell_t *this)
{
	lub_list_node_t *iter;
	clish_plugin_t *plugin;

	assert(this);

	/* Iterate elements */
	for(iter = lub_list__get_head(this->plugins);
		iter; iter = lub_list_node__get_next(iter)) {
		plugin = (clish_plugin_t *)lub_list_node__get_data(iter);
		if (!clish_plugin_load(plugin)) {
			fprintf(stderr, "Error: Can't load plugin %s.\n",
				clish_plugin__get_file(plugin));
			return -1;
		}
#ifdef DEBUG
		clish_plugin_dump(plugin);
#endif
	}

	return 0;
}

/*----------------------------------------------------------------------- */
/* Find plugin by name. */
static clish_plugin_t *plugin_by_name(clish_shell_t *this, const char *name)
{
	lub_list_node_t *iter;
	clish_plugin_t *plugin;

	/* Iterate elements */
	for(iter = lub_list__get_head(this->plugins);
		iter; iter = lub_list_node__get_next(iter)) {
		plugin = (clish_plugin_t *)lub_list_node__get_data(iter);
		if (!strcmp(clish_plugin__get_name(plugin), name))
			return plugin;
	}

	return NULL;
}

/*----------------------------------------------------------------------- */
/* Iterate plugins to find symbol by name.
 * The symbol name can be simple or with namespace:
 * mysym@plugin1
 * The symbols with prefix will be resolved using specified plugin only.
 */
static clish_plugin_fn_t *plugins_find_sym(clish_shell_t *this, const char *name)
{
	lub_list_node_t *iter;
	clish_plugin_t *plugin;
	clish_plugin_fn_t *func = NULL;
	/* To parse command name */
	char *saveptr;
	const char *delim = "@";
	char *plugin_name = NULL;
	char *cmdn = NULL;
	char *str = lub_string_dup(name);

	assert(this);

	/* Parse name to get sym name and optional plugin name */
	cmdn = strtok_r(str, delim, &saveptr);
	if (!cmdn)
		goto end;
	plugin_name = strtok_r(NULL, delim, &saveptr);

	if (plugin_name) {
		/* Search for symbol in specified plugin */
		plugin = plugin_by_name(this, plugin_name);
		if (!plugin)
			goto end;
		func = clish_plugin_get_sym(plugin, cmdn);
	} else {
		/* Iterate all plugins */
		for(iter = lub_list__get_head(this->plugins);
			iter; iter = lub_list_node__get_next(iter)) {
			plugin = (clish_plugin_t *)lub_list_node__get_data(iter);
			if ((func = clish_plugin_get_sym(plugin, cmdn)))
				break;
		}
	}
end:
	lub_string_free(str);
	return func;
}

/*--------------------------------------------------------- */
clish_sym_t *clish_shell_find_sym(clish_shell_t *this, const char *name)
{
	lub_list_node_t *iter;
	clish_sym_t *sym;

	/* Iterate elements */
	for(iter = lub_list__get_head(this->syms);
		iter; iter = lub_list_node__get_next(iter)) {
		int res;
		sym = (clish_sym_t *)lub_list_node__get_data(iter);
		res = strcmp(clish_sym__get_name(sym), name);
		if (!res)
			return sym;
		if (res > 0) /* No chances to find name */
			break;
	}

	return NULL;
}

/*----------------------------------------------------------------------- */
clish_sym_t *clish_shell_add_sym(clish_shell_t *this,
	clish_plugin_fn_t *func, const char *name)
{
	clish_sym_t *sym = NULL;

	if (!name)
		return NULL;
	if ((sym = clish_shell_find_sym(this, name)))
		return sym;
	if (!(sym = clish_sym_new(name, func)))
		return NULL;
	lub_list_add(this->syms, sym);

	return sym;
}

/*----------------------------------------------------------------------- */
clish_sym_t *clish_shell_add_unresolved_sym(clish_shell_t *this,
	const char *name)
{
	return clish_shell_add_sym(this, NULL, name);
}

/*----------------------------------------------------------------------- */
int clish_shell_link_plugins(clish_shell_t *this)
{
	clish_sym_t *sym;
	lub_list_node_t *iter;
	char *sym_name = NULL;
	clish_plugin_fn_t *fn = NULL;

	/* Iterate elements */
	for(iter = lub_list__get_head(this->syms);
		iter; iter = lub_list_node__get_next(iter)) {
//		int res;
		sym = (clish_sym_t *)lub_list_node__get_data(iter);
		sym_name = clish_sym__get_name(sym);
		fn = plugins_find_sym(this, sym_name);
		if (!fn) {
			fprintf(stderr, "Error: Can't resolve symbol %s.\n",
				sym_name);
//			return 0;
		}
//		fprintf(stderr, "sym: %s\n", sym_name);

	}

	return 0;
}