#ifndef _CLISH_PYTHON_H_
#define _CLISH_PYTHON_H_

#include "config.h"

#include <clish/shell.h>

#include <Python.h>

#define CLISH_PYTHON_UDATA_MODULE  "clish_python_module"
#define CLISH_PYTHON_UDATA_GLOBALS "clish_python_globals"
#define CLISH_PYTHON_UDATA_LOCALS  "clish_python_locals"

PyObject *clish_python_init(void);

PyObject *clish_python_shell_get_module(const clish_shell_t *shell);
PyObject *clish_python_shell_get_globals(const clish_shell_t *shell);
PyObject *clish_python_shell_get_locals(const clish_shell_t *shell);
int clish_python_shell_execute(clish_shell_t *shell, const char *script);

PyObject *clish_python_context_get_module(const clish_context_t *shell);
PyObject *clish_python_context_get_globals(const clish_context_t *context);
PyObject *clish_python_context_get_updated_locals(const clish_context_t *context);
int clish_python_context_execute(clish_context_t *context, const char *script);

extern PyTypeObject clish_python_type_action;
extern PyTypeObject clish_python_type_command;
extern PyTypeObject clish_python_type_context;
extern PyTypeObject clish_python_type_param;
extern PyTypeObject clish_python_type_paramv;
extern PyTypeObject clish_python_type_pargv;
extern PyTypeObject clish_python_type_ptype;
extern PyTypeObject clish_python_type_shell;
extern PyTypeObject clish_python_type_sym;
extern PyTypeObject clish_python_type_var;
extern PyTypeObject clish_python_type_vars;
extern PyTypeObject clish_python_type_view;

PyObject * clish_python_wrap_action(const clish_action_t *action);
PyObject * clish_python_wrap_command(const clish_command_t *command);
PyObject * clish_python_wrap_context(const clish_context_t *context);
PyObject * clish_python_wrap_param(const clish_param_t *param);
PyObject * clish_python_wrap_paramv(const clish_paramv_t *paramv);
PyObject * clish_python_wrap_pargv(const clish_pargv_t *pargv);
PyObject * clish_python_wrap_ptype(const clish_ptype_t *ptype);
PyObject * clish_python_wrap_shell(const clish_shell_t *command);
PyObject * clish_python_wrap_sym(const clish_sym_t *sym);
PyObject * clish_python_wrap_var(const clish_var_t *var);
PyObject * clish_python_wrap_view(const clish_view_t *view);

clish_action_t * clish_python_unwrap_action(PyObject *action);
clish_command_t * clish_python_unwrap_command(PyObject *command);
clish_context_t * clish_python_unwrap_context(PyObject *context);
clish_param_t * clish_python_unwrap_param(PyObject *param);
clish_paramv_t * clish_python_unwrap_paramv(PyObject *paramv);
clish_pargv_t * clish_python_unwrap_pargv(PyObject *pargv);
clish_ptype_t * clish_python_unwrap_ptype(PyObject *ptype);
clish_shell_t * clish_python_unwrap_shell(PyObject *shell);
clish_sym_t * clish_python_unwrap_sym(PyObject *sym);
clish_var_t * clish_python_unwrap_var(PyObject *var);
clish_view_t * clish_python_unwrap_view(PyObject *view);

PyObject * clish_python_wrap_as_vars(const clish_context_t *context);

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
#ifdef PYTHON3
		return PyUnicode_FromString(value);
#else
		return PyString_FromString(value);
#endif
	} else {
		return Py_None;
	}
}

static inline const char *clish_python_unwrap_string(PyObject *obj)
{
	if(obj) {
#ifdef PYTHON3
		return PyUnicode_AsUTF8(obj);
#else
		return PyString_AsString(obj);
#endif
	} else {
		return NULL;
	}
}

#endif /* _CLISH_PYTHON_H_ */
