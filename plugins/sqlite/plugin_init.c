
#include "private.h"

CLISH_PLUGIN_INIT(sqlite)
{
	int res;
	sqlite3 *db;
	clish_shell_t *shell = clish_shell;
	clish_sym_t *sym = NULL;

	/* register syms */
	clish_plugin__set_fini(plugin, clish_plugin_sqlite_fini);

	sym = clish_plugin_add_sym(plugin, clish_plugin_sqlite_action_sqlite, "sqlite");
	clish_sym__set_expand(sym, TRI_FALSE);

	clish_plugin_add_sym(plugin, clish_plugin_sqlite_action_sqlite_close, "sqlite_close");
	clish_plugin_add_sym(plugin, clish_plugin_sqlite_action_sqlite_open, "sqlite_open");

	/* XXX temporary hack */
	lub_info("Opening database");
	res = sqlite3_open("clish.db", &db);
	if(res != SQLITE_OK) {
		lub_error("Could not open database");
		return -1;
	}
	clish_shell__set_udata(shell, CLISH_SQLITE_UDATA_DATABASE, db);

	/* done */
	return 0;
}
