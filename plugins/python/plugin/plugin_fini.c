
#include <stdio.h>

#include "private.h"

CLISH_PLUGIN_FINI(clish_plugin_python_fini)
{
	clish_shell_t *shell = clish_shell;
	PyObject *p;

	/* clear udata */
	fprintf(stderr,"clish_plugin_python_fini: Clearing udata\n");
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
	fprintf(stderr,"clish_plugin_python_fini: Finalizing python\n");
	/* XXX should we always do this? */
	Py_Finalize();

	/* done */
	return (0);
}
