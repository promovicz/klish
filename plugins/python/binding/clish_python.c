
#include <stdio.h>

#include <lub/error.h>

#include "clish_python.h"

#define MODULE_NAME "clish"
#define MODULE_DOC  "API of the clish shell"

static PyMethodDef module_methods[] = {
	{NULL},
};

#ifdef PYTHON3
static PyModuleDef module_definition = {
	PyModuleDef_HEAD_INIT,
	.m_name = MODULE_NAME,
	.m_doc = MODULE_DOC,
	.m_size = -1,
	.m_methods = module_methods,
};
#endif

static int clish_python_init_class(PyObject *module, const char *name, PyTypeObject *type)
{
	int res;
	lub_trace("Registering class %s", name);
	/* prepare type */
	res = PyType_Ready(type);
	if (res < 0)
		return res;
	/* add type to module */
	PyModule_AddObject(module, name, (PyObject*)type);
	/* return */
	return 0;
}

PyObject *clish_python_init(void)
{
#ifdef PYTHON3
	int res;
#endif
	PyObject *module;

	/* python may or may not be initialized */
	if(Py_IsInitialized()) {
		lub_trace("Python already initialized");
	} else {
		lub_debug("Initializing python");
		Py_Initialize();
	}

	/* register module */
#ifdef PYTHON3
	lub_trace("Looking for existing module");
	module = PyState_FindModule(&module_definition);
	if(module) {
		lub_trace("Module already present");
		goto done;
	} else {
		lub_debug("Creating module instance");
		module = PyModule_Create(&module_definition);
		if(!module) {
			goto fail;
		}
		lub_trace("Adding module instance");
		res = PyState_AddModule(module, &module_definition);
		if(res) {
			goto fail;
		}
	}
#else
	lub_debug("Initializing module");
	module = Py_InitModule3(MODULE_NAME, module_methods, MODULE_DOC);
	if(!module) {
		lub_warning("Failed to initialize module");
		goto fail;
	}
#endif

	/* register classes */
	lub_debug("Registering classes");
	clish_python_init_class(module, "Action", &clish_python_type_action);
	clish_python_init_class(module, "Command", &clish_python_type_command);
	clish_python_init_class(module, "Context", &clish_python_type_context);
	clish_python_init_class(module, "Param", &clish_python_type_param);
	clish_python_init_class(module, "Paramv", &clish_python_type_paramv);
	clish_python_init_class(module, "Pargv", &clish_python_type_pargv);
	clish_python_init_class(module, "Ptype", &clish_python_type_ptype);
	clish_python_init_class(module, "Shell", &clish_python_type_shell);
	clish_python_init_class(module, "Sym", &clish_python_type_sym);
	clish_python_init_class(module, "Var", &clish_python_type_var);
	clish_python_init_class(module, "Vars", &clish_python_type_vars);
	clish_python_init_class(module, "View", &clish_python_type_view);

#if PYTHON3
done:
#endif
	lub_trace("Initialization complete");
	return module;

fail:
	lub_warning("Initialization failed");
	return NULL;
}


PyObject *clish_python_shell_get_module(const clish_shell_t *shell) {
	return clish_shell__get_udata(shell, CLISH_PYTHON_UDATA_MODULE);
}

PyObject *clish_python_shell_get_globals(const clish_shell_t *shell) {
	return clish_shell__get_udata(shell, CLISH_PYTHON_UDATA_GLOBALS);
}

PyObject *clish_python_shell_get_locals(const clish_shell_t *shell) {
	return clish_shell__get_udata(shell, CLISH_PYTHON_UDATA_LOCALS);
}

int clish_python_shell_execute(clish_shell_t *shell, const char *script) {
	PyObject *result;
	PyObject *globals = clish_python_shell_get_globals(shell);

	lub_debug("Executing: %s", script);

	result = PyRun_StringFlags(script, Py_file_input, globals, NULL, NULL);
	if(!result) {
		PyErr_Print();
		return -1;
	}

	Py_DECREF(result);

	return 0;
}

PyObject *clish_python_context_get_module(const clish_context_t *context) {
	const clish_shell_t *shell = clish_context__get_shell(context);
	return clish_shell__get_udata(shell, CLISH_PYTHON_UDATA_MODULE);
}

PyObject *clish_python_context_get_globals(const clish_context_t *context) {
	const clish_shell_t *shell = clish_context__get_shell(context);
	return clish_shell__get_udata(shell, CLISH_PYTHON_UDATA_GLOBALS);
}

PyObject *clish_python_context_get_updated_locals(const clish_context_t *context) {
	const clish_shell_t *shell = clish_context__get_shell(context);
	const clish_pargv_t *pargv = clish_context__get_pargv(context);
	const clish_action_t *action = clish_context__get_action(context);
	const clish_command_t *command = clish_context__get_cmd(context);

	lub_trace("Updating locals");

	PyObject *locals = clish_python_shell_get_locals(shell);

	PyDict_SetItemString(locals, "action", clish_python_wrap_action(action));
	PyDict_SetItemString(locals, "context", clish_python_wrap_context(context));
	PyDict_SetItemString(locals, "command", clish_python_wrap_command(command));
	PyDict_SetItemString(locals, "params", clish_python_wrap_pargv(pargv));
	PyDict_SetItemString(locals, "shell", clish_python_wrap_shell(shell));
	PyDict_SetItemString(locals, "vars", clish_python_wrap_as_vars(context));

	return locals;
}

int clish_python_context_execute(clish_context_t *context, const char *script) {
	PyObject *result;
	PyObject *globals = clish_python_context_get_globals(context);
	PyObject *locals = clish_python_context_get_updated_locals(context);

	lub_debug("Executing: %s", script);

	result = PyRun_StringFlags(script, Py_file_input, globals, locals, NULL);
	if(!result) {
		PyErr_Print();
		return -1;
	}

	Py_DECREF(result);

	return 0;
}
