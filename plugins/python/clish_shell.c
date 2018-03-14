
#include <clish/shell.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_shell_t *shell;
} python_shell_t;

#define SHELL_TYPE (&clish_python_type_shell)

PyTypeObject clish_python_type_shell = {
	PyObject_HEAD_INIT(NULL)
	0,                         /*ob_size*/
	"clish.Shell",             /*tp_name*/
	sizeof(python_shell_t),    /*tp_basicsize*/
	0,                         /*tp_itemsize*/
	0,                         /*tp_dealloc*/
	0,                         /*tp_print*/
	0,                         /*tp_getattr*/
	0,                         /*tp_setattr*/
	0,                         /*tp_compare*/
	0,                         /*tp_repr*/
	0,                         /*tp_as_number*/
	0,                         /*tp_as_sequence*/
	0,                         /*tp_as_mapping*/
	0,                         /*tp_hash */
	0,                         /*tp_call*/
	0,                         /*tp_str*/
	0,                         /*tp_getattro*/
	0,                         /*tp_setattro*/
	0,                         /*tp_as_buffer*/
	Py_TPFLAGS_DEFAULT,        /*tp_flags*/
	"the clish shell",         /*tp_doc*/
};


static PyObject *
shell_get_view(python_shell_t * self, void * closure)
{
	return clish_python_wrap_view
		(clish_shell__get_view(self->shell));
}

static PyObject *
shell_get_depth(python_shell_t * self, void * closure)
{
	return PyLong_FromUnsignedLong
		(clish_shell__get_depth(self->shell));
}

static PyObject *
shell_get_overview(python_shell_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_overview(self->shell));
}

static PyObject *
shell_get_lockfile(python_shell_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_lockfile(self->shell));
}

static PyObject *
shell_get_default_shebang(python_shell_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_default_shebang(self->shell));
}

static PyObject *
shell_get_pager_command(python_shell_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_pager_command(self->shell));
}

static PyObject *
shell_get_pager(python_shell_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_shell__get_pager(self->shell));
}

static PyObject *
shell_get_interactive(python_shell_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_shell__get_interactive(self->shell));
}

static PyObject *
shell_get_utf8(python_shell_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_shell__get_utf8(self->shell));
}

static PyObject *
shell_get_log(python_shell_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_shell__get_log(self->shell));
}

static PyObject *
shell_get_dryrun(python_shell_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_shell__get_dryrun(self->shell));
}

static PyGetSetDef shell_getset[] = {
	{"view", (getter)shell_get_view, NULL,
		"Current view"
	},
	{"depth", (getter)shell_get_depth, NULL,
		"Nesting depth"
	},
	{"overview", (getter)shell_get_overview, NULL,
		"Overview text"
	},
#if 0
	{"tinyrl", (getter)shell_get_tinyrl, NULL,
		"tinyrl instance used by the shell"
	},
#endif
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
	{"istream", (getter)shell_get_istream, NULL,
		"Input stream used by the shell"
	},
	{"ostream", (getter)shell_get_ostream, NULL,
		"Output stream used by the shell"
	},
#endif
	{"lockfile", (getter)shell_get_lockfile, NULL,
		"Lock file"
	},
#if 0
	{"state", (getter)shell_get_state, NULL,
		"State of the shell"
	},
#endif
	{"default_shebang", (getter)shell_get_default_shebang, NULL,
		"Default shebang for script actions"
	},
	{"pager_command", (getter)shell_get_pager_command, NULL,
		"Command for running the pager (sh)"
	},
	{"pager", (getter)shell_get_pager, NULL,
		"True if pager is enabled globally"
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
#if 0
	{"facility", (getter)shell_get_facility, NULL,
		"Log facility"
	},
#endif
	{"dryrun", (getter)shell_get_dryrun, NULL,
		"True if in dry-run mode"
	},
	{NULL}  /* Sentinel */
};

static PyObject *
shell_find_view(python_shell_t * self, PyObject * args)
{
	PyObject * result = Py_None;
	PyObject * name;
	if (PyArg_ParseTuple(args, "S", &name)) {
		clish_view_t * res = clish_shell_find_view(self->shell, PyString_AsString(name));
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
		clish_ptype_t * res = clish_shell_find_ptype(self->shell, PyString_AsString(name));
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
		clish_var_t * res = clish_shell_find_var(self->shell, PyString_AsString(name));
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

void clish_python_init_shell(PyObject *module, clish_shell_t *shell)
{
	PyTypeObject *type = SHELL_TYPE;
	python_shell_t *self;
	/* set type fields */
	type->tp_getset = shell_getset;
	type->tp_methods = shell_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* create instance */
	self = (python_shell_t*)PyType_GenericNew(type, Py_None, Py_None);
	self->shell = shell;
	/* add type to module */
	Py_INCREF((PyObject*)type);
	PyModule_AddObject(module, "Shell", (PyObject*)type);
	/* add instance to module */
	Py_INCREF((PyObject*)self);
	PyModule_AddObject(module, "shell", (PyObject*)self);
}

PyObject * clish_python_wrap_shell(const clish_shell_t *shell)
{
	if (shell) {
		PyTypeObject *type = SHELL_TYPE;
		python_shell_t *self;
		/* create instance */
		self = (python_shell_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->shell = shell;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_shell_t * clish_python_unwrap_shell(PyObject *shell)
{
	assert(PyObject_IsInstance(shell, SHELL_TYPE));
	python_shell_t *self = (python_shell_t*)shell;
	return self->shell;
}
