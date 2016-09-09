
#include <clish/command.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_command_t *command;
} python_command_t;

#define COMMAND_TYPE (&clish_python_type_command)

PyTypeObject clish_python_type_command = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Command",           /*tp_name*/
    sizeof(python_command_t),  /*tp_basicsize*/
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
    "clish command",           /*tp_doc*/
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

static PyObject *
command_get_name(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_name(self->command));
}

static PyObject *
command_get_suffix(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_suffix(self->command));
}

static PyObject *
command_get_text(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_text(self->command));
}

static PyObject *
command_get_detail(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_detail(self->command));
}

static PyObject *
command_get_escape_chars(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_escape_chars(self->command));
}

static PyObject *
command_get_regex_chars(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_regex_chars(self->command));
}

static PyObject *
command_get_args(python_command_t * self, void * closure)
{
	return clish_python_wrap_param
		(clish_command__get_args(self->command));
}

static PyObject *
command_get_action(python_command_t * self, void * closure)
{
	return clish_python_wrap_action
		(clish_command__get_action(self->command));
}

static PyObject *
command_get_viewname(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_viewname(self->command));
}

static PyObject *
command_get_viewid(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_viewid(self->command));
}

static PyObject *
command_get_params(python_command_t * self, void * closure)
{
	return clish_python_wrap_paramv
		(clish_command__get_paramv(self->command));
}

static PyObject *
command_get_pview(python_command_t * self, void * closure)
{
	return clish_python_wrap_view
		(clish_command__get_pview(self->command));
}

static PyObject *
command_get_depth(python_command_t * self, void * closure)
{
	return PyLong_FromUnsignedLong
		(clish_command__get_depth(self->command));
}

static PyObject *
command_get_lock(python_command_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_command__get_lock(self->command));
}

static PyObject *
command_get_orig(python_command_t * self, void * closure)
{
	return clish_python_wrap_command
		(clish_command__get_orig(self->command));
}

static PyObject *
command_get_cmd(python_command_t * self, void * closure)
{
	return clish_python_wrap_command
		(clish_command__get_cmd(self->command));
}

static PyObject *
command_get_alias(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_alias(self->command));
}

static PyObject *
command_get_alias_view(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_alias_view(self->command));
}

static PyObject *
command_get_dynamic(python_command_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_command__get_dynamic(self->command));
}

static PyObject *
command_get_interrupt(python_command_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_command__get_interrupt(self->command));
}

static PyObject *
command_get_pager(python_command_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_command__get_pager(self->command));
}

static PyObject *
command_get_access(python_command_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_command__get_access(self->command));
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
	{"params", (getter)command_get_params, NULL,
     "Return command params"
    },
	{"pview", (getter)command_get_pview, NULL,
     "Return command pview"
    },
	{"depth", (getter)command_get_depth, NULL,
     "Return command depth"
    },
#if 0
	{"restore", (getter)command_get_restore, NULL,
     "Return command restore"
    },
#endif
	{"orig", (getter)command_get_orig, NULL,
     "Return command orig"
    },
	{"cmd", (getter)command_get_cmd, NULL,
     "Return command cmd"
    },
	{"lock", (getter)command_get_lock, NULL,
     "Return command lock"
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
	{"interrupt", (getter)command_get_interrupt, NULL,
     "Return command interrupt"
    },
	{"pager", (getter)command_get_pager, NULL,
     "Return command pager"
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
	const char *name;
	PyObject *param_obj = Py_None;
	if (PyArg_ParseTuple(args, "O!", &clish_python_type_param, &param_obj)) {
		clish_param_t * param = clish_python_unwrap_param(param_obj);
		clish_command_insert_param(self->command, param);
	}
	return result;
}

static PyObject *
command_remove_param(python_command_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	const char *name;
	PyObject *param_obj = Py_None;
	if (PyArg_ParseTuple(args, "O!", &clish_python_type_param, &param_obj)) {
		clish_param_t * param = clish_python_unwrap_param(param_obj);
		clish_command_remove_param(self->command, param);
	}
	return result;
}

static PyMethodDef command_methods[] = {
	{"insert_param", (PyCFunction)command_insert_param, METH_VARARGS,
     "Insert a param"
	},
	{"remove_param", (PyCFunction)command_remove_param, METH_VARARGS,
     "Remove a param"
	},
	{NULL}  /* Sentinel */
};

void clish_python_init_command(PyObject *module)
{
	PyTypeObject *type = COMMAND_TYPE;
	/* set type fields */
	type->tp_new = command_new;
	type->tp_getset = command_getset;
	type->tp_methods = command_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "Command", (PyObject*)type);
}

PyObject * clish_python_wrap_command(const clish_command_t *command)
{
	if (command) {
		PyTypeObject *type = COMMAND_TYPE;
		python_command_t *self;
		/* create instance */
		self = (python_command_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->command = command;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_command_t * clish_python_unwrap_command(PyObject *command)
{
	assert(PyObject_IsInstance(command, COMMAND_TYPE));
	python_command_t *self = (python_command_t*)command;
	return self->command;
}
