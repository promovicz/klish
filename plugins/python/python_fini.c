
#include <stdio.h>

#include "private.h"

int clish_python_fini(clish_shell_t *shell)
{
	PyObject *params  = clish_shell__get_udata(shell, PYTHON_PARAMS_UDATA);
	PyObject *module  = clish_shell__get_udata(shell, PYTHON_MODULE_UDATA);

	clish_shell__set_udata(shell, PYTHON_PARAMS_UDATA, NULL);
	Py_DECREF(params);
	clish_shell__set_udata(shell, PYTHON_MODULE_UDATA, NULL);
	Py_DECREF(module);

	Py_Finalize();

	return 0;
}
