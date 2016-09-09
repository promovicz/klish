#ifndef _PLUGIN_H_
#define _PLUGIN_H_

#include "config.h"

#include <clish/shell.h>

#include <Python.h>

#define PYTHON_MODULE_UDATA "python_module"
#define PYTHON_PARAMS_UDATA "python_params"

extern clish_shell_t   *clish_python_shell;
extern clish_context_t *clish_python_context;

extern PyTypeObject clish_python_type_action;
extern PyTypeObject clish_python_type_command;
extern PyTypeObject clish_python_type_context;
extern PyTypeObject clish_python_type_param;
extern PyTypeObject clish_python_type_paramv;
extern PyTypeObject clish_python_type_ptype;
extern PyTypeObject clish_python_type_shell;
extern PyTypeObject clish_python_type_sym;
extern PyTypeObject clish_python_type_var;
extern PyTypeObject clish_python_type_view;

int clish_python_init(clish_shell_t *shell);
int clish_python_fini(clish_shell_t *shell);

void clish_python_init_shell(PyObject *module, clish_shell_t *shell);
void clish_python_init_action(PyObject *module);
void clish_python_init_command(PyObject *module);
void clish_python_init_context(PyObject *module);
void clish_python_init_param(PyObject *module);
void clish_python_init_paramv(PyObject *module);
void clish_python_init_pargv(PyObject *module);
void clish_python_init_ptype(PyObject *module);
void clish_python_init_sym(PyObject *module);
void clish_python_init_var(PyObject *module);
void clish_python_init_view(PyObject *module);

PyObject * clish_python_wrap_shell(const clish_shell_t *command);
PyObject * clish_python_wrap_action(const clish_action_t *action);
PyObject * clish_python_wrap_command(const clish_command_t *command);
PyObject * clish_python_wrap_param(const clish_param_t *param);
PyObject * clish_python_wrap_paramv(const clish_paramv_t *paramv);
PyObject * clish_python_wrap_pargv(const clish_pargv_t *pargv);
PyObject * clish_python_wrap_ptype(const clish_ptype_t *ptype);
PyObject * clish_python_wrap_sym(const clish_sym_t *sym);
PyObject * clish_python_wrap_var(const clish_var_t *var);
PyObject * clish_python_wrap_view(const clish_view_t *view);

clish_shell_t * clish_python_unwrap_shell(PyObject *shell);
clish_action_t * clish_python_unwrap_action(PyObject *action);
clish_command_t * clish_python_unwrap_command(PyObject *command);
clish_param_t * clish_python_unwrap_param(PyObject *param);
clish_paramv_t * clish_python_unwrap_paramv(PyObject *paramv);
clish_pargv_t * clish_python_unwrap_pargv(PyObject *pargv);
clish_ptype_t * clish_python_unwrap_ptype(PyObject *ptype);
clish_sym_t * clish_python_unwrap_sym(PyObject *sym);
clish_var_t * clish_python_unwrap_var(PyObject *var);
clish_view_t * clish_python_unwrap_view(PyObject *view);

void clish_python_update_context(PyObject *module, clish_context_t *context);

CLISH_PLUGIN_SYM(clish_plugin_python_action);
CLISH_PLUGIN_FINI(clish_plugin_python_fini);

static inline PyObject * clish_python_wrap_bool(bool_t value)
{
	if(value) {
		return Py_True;
	} else {
		return Py_False;
	}
}

static inline PyObject *clish_python_wrap_string(const char *value)
{
	if(value) {
		return PyString_FromString(value);
	} else {
		return Py_None;
	}
}

#endif /* _PLUGIN_H_ */
