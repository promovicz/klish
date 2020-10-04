
#include <stdlib.h>

#include "private.h"

typedef struct {
	const char *name;
	unsigned width;
	int type;
} clish_sqlite_column_t;

static int clish_sqlite_bind_pargv(sqlite3_stmt *st, clish_pargv_t *pargv) {
	int res, idx;
	unsigned u, n;
	lub_trace("Binding parameters");
	/* iterate over pargv */
	n = clish_pargv__get_count(pargv);
	for(u = 0; u < n; u++) {
		clish_parg_t *parg = clish_pargv__get_parg(pargv, u);
		const char *name = clish_parg__get_name(parg);
		const char *value = clish_parg__get_value(parg);
		char *xname = NULL;
		lub_trace("Scanning parameter %s = %s", name, value);
		/* prefix the name with @ */
		lub_string_cat(&xname, "@");
		lub_string_cat(&xname, name);
		/* bind the parameter */
		idx = sqlite3_bind_parameter_index(st, name);
		/* free temporary name immediately */
		lub_string_free(xname);
		/* bind the parameter if present */
		if(idx) {
			lub_debug("Found index %s at index %d", name, idx);
			/* bind the parameter */
			res = sqlite3_bind_text(st, idx, value, strlen(value), SQLITE_TRANSIENT);
			if(res != SQLITE_OK) {
				lub_error("Could not bind parameter %s (%d)", name, idx);
				return -1;
			}
		}
	}

	return 0;
}

static int clish_sqlite_prepare_columns(sqlite3_stmt *st, clish_sqlite_column_t **colsp) {
	int i, numc;
	clish_sqlite_column_t *cols = NULL;

	lub_trace("Preparing columns");

	numc = sqlite3_column_count(st);
	cols = calloc(numc, sizeof(clish_sqlite_column_t));

	for(i = 0; i < numc; i++) {
		const char *cn = sqlite3_column_name(st, i);
		lub_trace("Column %d name \"%s\"", i, cn);
		cols[i].name = cn;
		cols[i].width = 8;
	}

	*colsp = cols;

	return numc;
}

static int clish_sqlite_print_columns(sqlite3_stmt *st, clish_sqlite_column_t *cols, int numc, int row) {
	int i;
	bool_t grown = BOOL_FALSE;
	const char **vals;

	/* allocate temporary array */
	vals = calloc(numc, sizeof(char*));

	/* determine column types */
	if(row == 0) {
		for(i = 0; i < numc; i++) {
			const char *cn = cols[i].name;
			int ct = sqlite3_column_type(st, i);
			cols[i].type = ct;
			lub_trace("Column %d name \"%s\" type %d", i, cn, ct);
		}
	}

	/* get values as text, adjust width accordingly */
	for(i = 0; i < numc; i++) {
		const char *val = sqlite3_column_text(st, i);
		size_t vlen = strlen(val);
		if(vlen > cols[i].width) {
			grown = BOOL_TRUE;
			cols[i].width = vlen;
		}
		vals[i] = val;
	}

	/* print headings, padding to width */
	if(row == 0) {
		for(i = 0; i < numc; i++) {
			const char *val = cols[i].name;
			size_t vlen = strlen(val);
			size_t pad = cols[i].width - vlen;
			if(i == 0) {
				fputc(' ', stdout);
				fputc('|', stdout);
			}
			fputc(' ', stdout);
			fwrite(val, 1, vlen, stdout);
			while(pad--) {
				fputc(' ', stdout);
			}
			fputc(' ', stdout);
			fputc('|', stdout);
		}
		fputc('\n', stdout);
	}

	/* print separator */
	if(row == 0) {
		for(i = 0; i < numc; i++) {
			size_t pad = cols[i].width;
			if(i == 0) {
				fputc(' ', stdout);
				fputc('+', stdout);
			}
			fputc('-', stdout);
			while(pad--) {
				fputc('-', stdout);
			}
			fputc('-', stdout);
			fputc('+', stdout);
		}
		fputc('\n', stdout);
	}

	/* print each value, padding to width */
	for(i = 0; i < numc; i++) {
		const char *val = vals[i];
		size_t vlen = strlen(val);
		size_t pad = cols[i].width - vlen;
		if(i == 0) {
			fputc(' ', stdout);
			fputc('|', stdout);
		}
		fputc(' ', stdout);
		fwrite(val, 1, vlen, stdout);
		while(pad--) {
			fputc(' ', stdout);
		}
		fputc(' ', stdout);
		fputc('|', stdout);
	}
	fputc('\n', stdout);

	/* free temp */
	free(vals);

	return numc;
}

static int clish_sqlite_finish_columns(sqlite3_stmt *st, clish_sqlite_column_t *cols, int numc) {
	int i;

	for(i = 0; i < numc; i++) {
		for(i = 0; i < numc; i++) {
			size_t pad = cols[i].width;
			if(i == 0) {
				fputc(' ', stdout);
				fputc('+', stdout);
			}
			fputc('-', stdout);
			while(pad--) {
				fputc('-', stdout);
			}
			fputc('-', stdout);
			fputc('+', stdout);
		}
		fputc('\n', stdout);
	}

	return numc;
}

CLISH_PLUGIN_SYM(clish_plugin_sqlite_action_sqlite)
{
	int res, nump, numc, row;
	sqlite3 *db;
	sqlite3_stmt *st;
	bool_t fail = BOOL_FALSE;
	const char *sql;
	clish_sqlite_column_t *cols = NULL;
	clish_shell_t *shell = clish_context__get_shell(clish_context);
	clish_pargv_t *pargv = clish_context__get_pargv(clish_context);

	/* find code to execute */
	sql = clish_pargv_find_value(pargv, "code");
	if(!sql) {
		sql = script;
	}
	if(!sql) {
		lub_error("No command provided.");
		return -1;
	}

	/* get the database */
	db = (sqlite3*)clish_shell__get_udata(shell, CLISH_SQLITE_UDATA_DATABASE);
	if(!db) {
		lub_error("No database!?");
		return -1;
	}

	/* execute sql while there is code */
	while(sql && !fail) {

		/* skip whitespace */
		sql += strspn(sql, "\t\n ");

		/* stop if at end */
		if(!*sql) {
			break;
		}

		/* prepare next available statement */
		lub_debug("Preparing statement \"%s\"", sql);
		res = sqlite3_prepare(db, sql, strlen(sql), &st, &sql);
		if(res != SQLITE_OK) {
			lub_error("Error preparing statement!?");
			fail = BOOL_TRUE;
			goto out;
		}

		/* check for bindable parameters */
		nump = sqlite3_bind_parameter_count(st);
		lub_trace("Have %d bindable params", nump);

		/* bind parameters */
		if(pargv && nump > 0) {
			res = clish_sqlite_bind_pargv(st, pargv);
			if(res) {
				fail = BOOL_TRUE;
				goto out;
			}
		}

		/* check for returned columns */
		numc = sqlite3_column_count(st);
		lub_trace("Have %d returned columns", numc);

		/* prepare for returned columns */
		if(numc > 0) {
			res = clish_sqlite_prepare_columns(st, &cols);
			if(res < 0 || res != numc) {
				fail = BOOL_TRUE;
				goto out;
			}
		}

		/* execute statement and iterate data */
		row = 0;
		while((res = sqlite3_step(st)) == SQLITE_ROW) {
			if(numc > 0) {
				clish_sqlite_print_columns(st, cols, numc, row);
			}
			row++;
		}
		switch(res) {
		case SQLITE_OK:
		case SQLITE_DONE:
			break;
		case SQLITE_BUSY:
			// abort safely, can't wait and retry without complex logic
		default:
			fail = BOOL_TRUE;
			break;
		}

	out:
		/* finish printing and drop columns */
		if(numc > 0) {
			clish_sqlite_finish_columns(st, cols, numc);
		}

		/* finalize the current statement */
		res = sqlite3_finalize(st);
		if(res != SQLITE_OK) {
			lub_error("Error finalizing statement!?");
			fail = BOOL_TRUE;
		}
	}

	/* done */
	return fail ? -1 : 0;
}
