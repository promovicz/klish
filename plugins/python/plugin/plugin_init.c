
#include <stdio.h>

#include "lub/ini.h"
#include "lub/string.h"

#include "private.h"

#include "pyerrors.h"

CLISH_PLUGIN_INIT(python)
{
	int res;
	const char *config;
	PyObject *pymodule, *pymain, *pyglobals, *pylocals;
	clish_shell_t *shell = clish_shell;
	clish_sym_t *sym = NULL;

	/* set the program name if appropriate */
	lub_trace("Setting program name");
	if(!Py_IsInitialized()) {
#ifdef PYTHON3
		//Py_SetProgramName(PyUnicode_AsWideCharString("clish", NULL));
#else
		Py_SetProgramName("clish");
#endif
	}

	/* initialize the binding */
	lub_trace("Initializing the binding");
	pymodule = clish_python_init();
	pymain = PyImport_AddModule("__main__");
	pylocals = PyDict_New();
	pyglobals = PyModule_GetDict(pymain);
	PyDict_SetItemString(pyglobals, "__builtin__", PyEval_GetBuiltins());
	PyDict_SetItemString(pyglobals, "clish", pymodule);

	/* register udata */
	lub_trace("Registering udata");
	clish_shell__set_udata(shell, CLISH_PYTHON_UDATA_MODULE, pymodule);
	clish_shell__set_udata(shell, CLISH_PYTHON_UDATA_GLOBALS, pyglobals);
	clish_shell__set_udata(shell, CLISH_PYTHON_UDATA_LOCALS, pylocals);

	/* register actions */
	lub_trace("Registering actions");
	sym = clish_plugin_add_sym(plugin, clish_plugin_python_action_python, "python");
	clish_sym__set_expand(sym, TRI_FALSE);

	/* run the config string */
	lub_trace("Configuring");
	config = clish_plugin__get_conf(plugin);
	if (config) {
		res = clish_python_shell_execute(shell, config);
		if(res) {
			return res;
		}
	}

	/* done */
	return 0;
}
