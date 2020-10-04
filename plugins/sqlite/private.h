#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include <string.h>

#include <lub/error.h>
#include <lub/string.h>

#include <clish/shell.h>

#include <sqlite3.h>

#define CLISH_SQLITE_UDATA_DATABASE  "clish_sqlite_database"

CLISH_PLUGIN_SYM(clish_plugin_sqlite_action_sqlite);
CLISH_PLUGIN_SYM(clish_plugin_sqlite_action_sqlite_close);
CLISH_PLUGIN_SYM(clish_plugin_sqlite_action_sqlite_open);
CLISH_PLUGIN_FINI(clish_plugin_sqlite_fini);

#endif /* _PLUGIN_H_ */
