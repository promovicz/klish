
#include <clish/shell.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_context_t *context;
} python_context_t;

#define CONTEXT_TYPE (&clish_python_type_context)

PyTypeObject clish_python_type_context = {
	PyObject_HEAD_INIT(NULL)
	0,                         /*ob_size*/
	"clish.Context",           /*tp_name*/
	sizeof(python_context_t),  /*tp_basicsize*/
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
	"clish context",           /*tp_doc*/
};

static PyObject *
context_get_shell(python_context_t * self, void * closure)
{
	return clish_python_wrap_shell
		(clish_context__get_shell(self->context));
}

static PyObject *
context_get_command(python_context_t * self, void * closure)
{
	return clish_python_wrap_command
		(clish_context__get_cmd(self->context));
}

static PyObject *
context_get_pargv(python_context_t * self, void * closure)
{
	return clish_python_wrap_pargv
		(clish_context__get_pargv(self->context));
}

static PyObject *
context_get_view(python_context_t * self, void * closure)
{
	return clish_python_wrap_view
		(clish_shell__get_view(clish_context__get_shell(self->context)));
}

static PyObject *
context_get_line(python_context_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_line(self->context));
}

static PyObject *
context_get_full_line(python_context_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_full_line(self->context));
}

static PyObject *
context_get_params_line(python_context_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_shell__get_params(self->context));
}

static PyGetSetDef context_getset[] = {
	{"shell", (getter)context_get_shell, NULL,
		"The shell"
	},
	{"command", (getter)context_get_command, NULL,
		"The current command"
	},
	{"params", (getter)context_get_pargv, NULL,
		"The current parameters as a pargv"
	},
	{"view", (getter)context_get_view, NULL,
		"The current view"
	},
	{"line", (getter)context_get_line, NULL,
		"Command part of current command"
	},
	{"full_line", (getter)context_get_full_line, NULL,
		"The current command"
	},
	{"params_line", (getter)context_get_params_line, NULL,
		"Parameter part of current command"
	},
	{NULL}  /* Sentinel */
};

void clish_python_init_context(PyObject *module)
{
	PyTypeObject *type = CONTEXT_TYPE;
	/* set type fields */
	type->tp_getset = context_getset;
	/* prepare type */
	if (PyType_Ready(type) < 0)
		return;
	/* add type to module */
	Py_INCREF((PyObject*)type);
	PyModule_AddObject(module, "Context", (PyObject*)type);
}

void clish_python_update_context(PyObject *module, clish_context_t *context)
{
	PyTypeObject *type = CONTEXT_TYPE;
	python_context_t *self;
	/* create instance */
	self = (python_context_t*)PyType_GenericNew(type, Py_None, Py_None);
	self->context = context;
	/* add instance to module */
	Py_INCREF((PyObject*)self);
	PyModule_AddObject(module, "context", (PyObject*)self);
}

clish_context_t * clish_python_unwrap_context(PyObject *context)
{
	assert(PyObject_IsInstance(context, CONTEXT_TYPE));
	python_context_t *self = (python_context_t*)context;
	return self->context;
}
