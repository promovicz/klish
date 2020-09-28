
#include <clish/shell.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_shell_t *shell;
} python_shell_t;

#define SHELL_TYPE (&clish_python_type_shell)

CLISH_PYTHON_WRAP(shell);
CLISH_PYTHON_UNWRAP(shell);

CLISH_PYTHON_GET(shell, view, view);
CLISH_PYTHON_GET_STRING(shell, overview);
CLISH_PYTHON_GET_STRING(shell, lockfile);
CLISH_PYTHON_GET_STRING(shell, default_shebang);
CLISH_PYTHON_GET_BOOL(shell, interactive);
CLISH_PYTHON_GET_BOOL(shell, utf8);
CLISH_PYTHON_GET_BOOL(shell, log);
CLISH_PYTHON_GET_BOOL(shell, dryrun);

static PyObject *
shell_get_depth(python_shell_t * self, void * closure)
{
	return PyLong_FromUnsignedLong
		(clish_shell__get_depth(self->shell));
}

static PyGetSetDef shell_getset[] = {
	{"view", (getter)shell_get_view, NULL,
	 "Current view"
	},
	{"overview", (getter)shell_get_overview, NULL,
	 "Overview text"
	},
	{"lockfile", (getter)shell_get_lockfile, NULL,
	 "Lock file"
	},
	{"default_shebang", (getter)shell_get_default_shebang, NULL,
	 "Default shebang for script actions"
	},
	{"interactive", (getter)shell_get_interactive, NULL,
	 "True if running interactively"
	},
	{"utf8", (getter)shell_get_utf8, NULL,
	 "True if UTF-8 is enabled"
	},
	{"log", (getter)shell_get_log, NULL,
	 "True if logging enabled"
	},
	{"dryrun", (getter)shell_get_dryrun, NULL,
	 "True if in dry-run mode"
	},
	{"depth", (getter)shell_get_depth, NULL,
	 "Nesting depth"
	},
#if 0
	{"pwd_line", (getter)shell_get_pwd_line, NULL,
	 "PWD line"
	},
	{"pwd_pargv", (getter)shell_get_pwd_pargv, NULL,
	 "PWD line"
	},
	{"pwd_cmd", (getter)shell_get_pwd_cmd, NULL,
	 "PWD line"
	},
	{"pwd_prefix", (getter)shell_get_pwd_prefix, NULL,
	 "PWD prefix"
	},
	{"pwd_full", (getter)shell_get_pwd_full, NULL,
	 "PWD full"
	},
	{"pwd_view", (getter)shell_get_pwd_view, NULL,
	 "PWD view"
	},
#endif
#if 0
	{"client", (getter)shell_get_client, NULL,
	 "konf client used by the shell"
	},
#endif
#if 0
	{"tinyrl", (getter)shell_get_tinyrl, NULL,
	 "tinyrl instance used by the shell"
	},
#endif
#if 0
	{"istream", (getter)shell_get_istream, NULL,
	 "Input stream used by the shell"
	},
	{"ostream", (getter)shell_get_ostream, NULL,
	 "Output stream used by the shell"
	},
#endif
#if 0
	{"state", (getter)shell_get_state, NULL,
	 "State of the shell"
	},
#endif
#if 0
	{"pager_command", (getter)shell_get_pager_command, NULL,
	 "Command for running the pager (sh)"
	},
	{"pager", (getter)shell_get_pager, NULL,
	 "True if pager is enabled globally"
	},
#endif
#if 0
	{"facility", (getter)shell_get_facility, NULL,
	 "Log facility"
	},
#endif
	{NULL}  /* Sentinel */
};

static PyObject *
shell_find_view(python_shell_t * self, PyObject * args)
{
	PyObject * result = Py_None;
	PyObject * name;
	if (PyArg_ParseTuple(args, "S", &name)) {
		clish_view_t * res = clish_shell_find_view(self->shell, clish_python_unwrap_string(name));
		if(res) {
			result = clish_python_wrap_view(res);
		}
	}
	return result;
}

static PyObject *
shell_find_ptype(python_shell_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	PyObject *name;
	if (PyArg_ParseTuple(args, "S", &name)) {
		clish_ptype_t * res = clish_shell_find_ptype(self->shell, clish_python_unwrap_string(name));
		if(res) {
			result = clish_python_wrap_ptype(res);
		}
	}
	return result;
}

#if 0
static PyObject *
shell_find_var(python_shell_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	PyObject *name;
	if (PyArg_ParseTuple(args, "S", &name)) {
		clish_var_t * res = clish_shell_find_var(self->shell, clish_python_unwrap_string(name));
		if(res) {
			result = clish_python_wrap_var(res);
		}
	}
	return result;
}
#endif

static PyMethodDef shell_methods[] = {
	{"find_view", (PyCFunction)shell_find_view, METH_VARARGS,
	 "Find named view"
	},
	{"find_ptype", (PyCFunction)shell_find_ptype, METH_VARARGS,
	 "Find named ptype"
	},
#if 0
	{"find_var", (PyCFunction)shell_find_var, METH_VARARGS,
	 "Find named variable"
	},
#endif
#if 0
	{"find_plugin", (PyCFunction)shell_find_plugin, METH_VARARGS,
	 "Find named plugin"
	},
#endif
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_shell = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Shell",
	.tp_doc = "the clish shell",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_shell_t),
	.tp_getset = shell_getset,
	.tp_methods = shell_methods,
};
