#include <stdio.h>
#include <stdlib.h>
#include <sys/wait.h>
#include <unistd.h>

#include <konf/buf.h>
#include <lub/string.h>

#include "private.h"

clish_shell_t   *clish_python_shell;
clish_context_t *clish_python_context;

CLISH_PLUGIN_SYM(clish_plugin_python_action)
{
	unsigned u, n;
	clish_shell_t   *shell   = clish_context__get_shell(clish_context);
	clish_pargv_t   *pargv  = clish_context__get_pargv(clish_context);
	PyObject *module = clish_shell__get_udata(shell, PYTHON_MODULE_UDATA);
	PyObject *params = clish_shell__get_udata(shell, PYTHON_PARAMS_UDATA);

	/* update context */
	clish_python_update_context(module, clish_context);

	/* update parameters */
	PyDict_Clear(params);
	n = clish_pargv__get_count(pargv);
	for(u = 0; u < n; u++) {
		clish_parg_t *parg = clish_pargv__get_parg(pargv, u);
		PyObject *name = clish_python_wrap_string(clish_parg__get_name(parg));
		PyObject *value = clish_python_wrap_string(clish_parg__get_value(parg));
		PyDict_SetItem(params, name, value);
	}

	/* succeed if there is no script */
	if (!script)
		return (0);

	/* execute the code */
	clish_python_shell   = shell;
	clish_python_context = clish_context;
	PyRun_SimpleString(script);
	clish_python_shell   = NULL;
	clish_python_context = NULL;

	return 0;
}
