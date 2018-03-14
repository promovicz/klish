#include <dirent.h>
#include <stdlib.h>
#include <string.h>

#include <lub/string.h>

#include "private.h"

static PyMethodDef module_methods[] = {
	{NULL},
};

int clish_python_init(clish_shell_t *shell)
{
	PyObject *module;
	PyObject *params;

	/* set the program name */
	Py_SetProgramName("clish");

	/* initialize python interpreter */
	Py_Initialize();

	/* create clish module */
	module = Py_InitModule3("clish", module_methods, "API of the clish shell");

	/* create parameters object */
	params = PyDict_New();
	/* register parameters object */
	Py_INCREF(params);
	PyModule_AddObject(module, "params", params);

	/* remember module as user data */
	Py_INCREF(module);
	clish_shell__set_udata(shell, PYTHON_MODULE_UDATA, module);
	Py_INCREF(params);
	clish_shell__set_udata(shell, PYTHON_PARAMS_UDATA, params);

	/* register classes */
	clish_python_init_shell(module, shell);
	clish_python_init_action(module);
	clish_python_init_command(module);
	clish_python_init_context(module);
	clish_python_init_param(module);
	clish_python_init_paramv(module);
	clish_python_init_pargv(module);
	clish_python_init_ptype(module);
	clish_python_init_sym(module);
	clish_python_init_var(module);
	clish_python_init_view(module);

	/* import the module */
	PyRun_SimpleString("import clish");

	return (0);
}

