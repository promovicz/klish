
#include <clish/view.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_view_t *view;
} python_view_t;

#define VIEW_TYPE (&clish_python_type_view)

PyTypeObject clish_python_type_view = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.View",              /*tp_name*/
    sizeof(python_view_t),     /*tp_basicsize*/
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
    "clish view",              /*tp_doc*/
};

static PyObject *
view_wrap_restore(clish_view_restore_e restore)
{
	return clish_python_wrap_string
		(clish_view_restore__get_name(restore));
}

static clish_view_restore_e
view_unwrap_restore(const char * restore)
{
	return clish_view_restore_resolve(restore);
}

static PyObject * view_new(PyTypeObject * subtype,
						   PyObject * args,
						   PyObject * kwds)
{
	const char *name = NULL;
	const char *prompt = NULL;
	if (PyArg_ParseTuple(args, "ss", &name, &prompt)) {
		clish_view_t * view;
		view = clish_view_new(name, prompt);
		return clish_python_wrap_view(view);
	}
	return NULL;
}

static PyObject *
view_get_name(python_view_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_view__get_name(self->view));
}

static PyObject *
view_get_prompt(python_view_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_view__get_prompt(self->view));
}

static PyObject *
view_get_depth(python_view_t * self, void * closure)
{
	return PyLong_FromUnsignedLong
		(clish_view__get_depth(self->view));
}

static PyObject *
view_get_restore(python_view_t * self, void * closure)
{
	return view_wrap_restore
		(clish_view__get_restore(self->view));
}

static PyObject *
view_get_access(python_view_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_view__get_access(self->view));
}

static PyGetSetDef view_getset[] = {
	{"name", (getter)view_get_name, NULL,
     "Return view name"
    },
	{"prompt", (getter)view_get_prompt, NULL,
     "Return view prompt"
    },
	{"depth", (getter)view_get_depth, NULL,
     "Return view depth"
    },
	{"restore", (getter)view_get_restore, NULL,
     "Return restore mode"
    },
	{"access", (getter)view_get_access, NULL,
     "Return view access"
    },
    {NULL}  /* Sentinel */
};

static PyObject *
view_new_command(python_view_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	const char *name;
	const char *help;
	if (PyArg_ParseTuple(args, "ss", &name, &help)) {
		clish_command_t * res = clish_view_new_command
			(self->view, name, help);
		if(res) {
			result = clish_python_wrap_command(res);
		}
	}
	return result;
}

static PyObject *
view_new_command_link(python_view_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	const char *name;
	const char *help;
	PyObject *ref;
	if (PyArg_ParseTuple(args, "ssO!", &name, &help,
						 &clish_python_type_command, &ref)) {
		clish_command_t * refcmd = clish_python_unwrap_command(ref);
		clish_command_t * res = clish_view_new_command_link
			(self->view, name, help, refcmd);
		if(res) {
			result = clish_python_wrap_command(res);
		}
	}
	return result;
}

static PyObject *
view_find_command(python_view_t * self, PyObject * args)
{
	PyObject *result = Py_None;
	const char *name;
	PyObject *inherit = Py_True;
	if (PyArg_ParseTuple(args, "s|O", &name, &inherit)) {
		clish_command_t * res = clish_view_find_command
			(self->view,
			 name,
			 PyObject_IsTrue(inherit)?BOOL_TRUE:BOOL_FALSE);
		if(res) {
			result = clish_python_wrap_command(res);
		}
	}
	return result;
}

static PyMethodDef view_methods[] = {
	{"new_command", (PyCFunction)view_new_command, METH_VARARGS,
     "Create new command in this view"
	},
	{"new_command_link", (PyCFunction)view_new_command_link, METH_VARARGS,
     "Create new linked command in this view"
	},
	{"find_command", (PyCFunction)view_find_command, METH_VARARGS,
     "Find named command, second argument controls inheritance"
	},
	{NULL}  /* Sentinel */
};

void clish_python_init_view(PyObject *module)
{
	PyTypeObject *type = VIEW_TYPE;
	/* set type fields */
	type->tp_new = view_new;
	type->tp_getset = view_getset;
	type->tp_methods = view_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "View", (PyObject*)type);
}

PyObject * clish_python_wrap_view(const clish_view_t *view)
{
	if (view) {
		PyTypeObject *type = VIEW_TYPE;
		python_view_t *self;
		/* create instance */
		self = (python_view_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->view = view;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_view_t * clish_python_unwrap_view(PyObject *view)
{
	assert(PyObject_IsInstance(view, VIEW_TYPE));
	python_view_t *self = (python_view_t*)view;
	return self->view;
}
