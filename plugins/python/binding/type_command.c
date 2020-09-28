
#include <clish/command.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_command_t *command;
} python_command_t;

#define COMMAND_TYPE (&clish_python_type_command)

CLISH_PYTHON_WRAP(command);
CLISH_PYTHON_UNWRAP(command);

CLISH_PYTHON_GET_STRING(command, name);
CLISH_PYTHON_GET_STRING(command, suffix);
CLISH_PYTHON_GET_STRING(command, text);
CLISH_PYTHON_GET_STRING(command, detail);
CLISH_PYTHON_GET_STRING(command, escape_chars);
CLISH_PYTHON_GET_STRING(command, regex_chars);
CLISH_PYTHON_GET(command, param, args);
CLISH_PYTHON_GET(command, action, action);
CLISH_PYTHON_GET_STRING(command, viewname);
CLISH_PYTHON_GET_STRING(command, viewid);
CLISH_PYTHON_GET(command, paramv, paramv);
CLISH_PYTHON_GET(command, view, pview);
CLISH_PYTHON_GET(command, command, orig);
CLISH_PYTHON_GET(command, command, cmd);
CLISH_PYTHON_GET_STRING(command, alias);
CLISH_PYTHON_GET_STRING(command, alias_view);
CLISH_PYTHON_GET_BOOL(command, dynamic);
CLISH_PYTHON_GET_STRING(command, access);

static PyObject *
command_get_depth(python_command_t * self, void * closure)
{
	return PyLong_FromUnsignedLong
		(clish_command__get_depth(self->command));
}

static PyGetSetDef command_getset[] = {
	{"name", (getter)command_get_name, NULL,
	 "Return command name"
	},
	{"suffix", (getter)command_get_suffix, NULL,
	 "Return command suffix"
	},
	{"text", (getter)command_get_text, NULL,
	 "Return command text"
	},
	{"detail", (getter)command_get_detail, NULL,
	 "Return command detail"
	},
	{"escape_chars", (getter)command_get_escape_chars, NULL,
	 "Return command escape characters"
	},
	{"regex_chars", (getter)command_get_regex_chars, NULL,
	 "Return command regex characters"
	},
	{"args", (getter)command_get_args, NULL,
	 "Return command args"
	},
	{"action", (getter)command_get_action, NULL,
	 "Return command action"
	},
	{"viewname", (getter)command_get_viewname, NULL,
	 "Return command viewname"
	},
	{"viewid", (getter)command_get_viewid, NULL,
	 "Return command viewid"
	},
	{"params", (getter)command_get_paramv, NULL,
	 "Return command paramv"
	},
	{"pview", (getter)command_get_pview, NULL,
	 "Return command pview"
	},
	{"depth", (getter)command_get_depth, NULL,
	 "Return command depth"
	},
	{"orig", (getter)command_get_orig, NULL,
	 "Return command orig"
	},
	{"cmd", (getter)command_get_cmd, NULL,
	 "Return command cmd"
	},
	{"alias", (getter)command_get_alias, NULL,
	 "Return command alias"
	},
	{"alias_view", (getter)command_get_alias_view, NULL,
	 "Return command alias view"
	},
	{"dynamic", (getter)command_get_dynamic, NULL,
	 "Return command dynamic"
	},
	{"access", (getter)command_get_access, NULL,
	 "Return command access"
	},
	{NULL}  /* Sentinel */
};

static PyObject *
command_insert_param(python_command_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	PyObject *param_obj = Py_None;
	if (PyArg_ParseTuple(args, "O!", &clish_python_type_param, &param_obj)) {
		clish_param_t * param = clish_python_unwrap_param(param_obj);
		clish_command_insert_param(self->command, param);
	}
	return result;
}

#if 0
static PyObject *
command_remove_param(python_command_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	PyObject *param_obj = Py_None;
	if (PyArg_ParseTuple(args, "O!", &clish_python_type_param, &param_obj)) {
		clish_param_t * param = clish_python_unwrap_param(param_obj);
		clish_command_remove_param(self->command, param);
	}
	return result;
}
#endif

static PyMethodDef command_methods[] = {
	{"insert_param", (PyCFunction)command_insert_param, METH_VARARGS,
	 "Insert a param"
	},
#if 0
	{"remove_param", (PyCFunction)command_remove_param, METH_VARARGS,
	 "Remove a param"
	},
#endif
	{NULL}  /* Sentinel */
};

static PyObject * command_new(PyTypeObject * subtype,
			      PyObject * args,
			      PyObject * kwds)
{
	const char *name = NULL;
	const char *help = NULL;
	python_command_t *ref = NULL;
	if (PyArg_ParseTuple(args, "ss|O!", &name, &help, COMMAND_TYPE, &ref)) {
		clish_command_t * command;
		if(ref) {
			command = clish_command_new_link(name, help, ref->command);
		} else {
			command = clish_command_new(name, help);
		}
		return clish_python_wrap_command(command);
	}
	return NULL;
}

PyTypeObject clish_python_type_command = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Command",
	.tp_doc = "clish command",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_command_t),
	.tp_new = command_new,
	.tp_getset = command_getset,
	.tp_methods = command_methods,
};
