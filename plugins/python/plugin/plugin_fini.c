
#include <stdio.h>

#include "private.h"

CLISH_PLUGIN_FINI(clish_plugin_python_fini)
{
	clish_shell_t *shell = clish_shell;
	PyObject *p;

	/* clear udata */
	lub_trace("Clearing udata");
	p = clish_shell__del_udata(shell, CLISH_PYTHON_UDATA_MODULE);
	if(p) {
		Py_DECREF(p);
	}
	p = clish_shell__del_udata(shell, CLISH_PYTHON_UDATA_GLOBALS);
	if(p) {
		Py_DECREF(p);
	}
	p = clish_shell__del_udata(shell, CLISH_PYTHON_UDATA_LOCALS);
	if(p) {
		Py_DECREF(p);
	}

	/* finalize python */
	/* XXX should we always do this? */
	lub_trace("Finalizing python");
	Py_Finalize();

	/* done */
	return (0);
}
