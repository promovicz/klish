
#include <clish/view.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_view_t *view;
} python_view_t;

#define VIEW_TYPE (&clish_python_type_view)

CLISH_PYTHON_WRAP(view);
CLISH_PYTHON_UNWRAP(view);

CLISH_PYTHON_GET_STRING(view, name);
CLISH_PYTHON_GET_STRING(view, prompt);
CLISH_PYTHON_GET_STRING(view, access);

static PyObject *
view_wrap_view_restore(clish_view_restore_e restore)
{
	return clish_python_wrap_string
		(clish_view_restore__get_name(restore));
}

static clish_view_restore_e
view_unwrap_view_restore(PyObject *string)
{
	return clish_view_restore_resolve
		(clish_python_unwrap_string(string));
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
	return view_wrap_view_restore
		(clish_view__get_restore(self->view));
}

static PyGetSetDef view_getset[] = {
	{"name", (getter)view_get_name, NULL,
	 "Return view name"
	},
	{"prompt", (getter)view_get_prompt, NULL,
	 "Return view prompt"
	},
	{"access", (getter)view_get_access, NULL,
	 "Return view access"
	},
	{"depth", (getter)view_get_depth, NULL,
	 "Return view depth"
	},
	{"restore", (getter)view_get_restore, NULL,
	 "Return restore mode"
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

#if 0
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
#endif

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
#if 0
	{"new_command_link", (PyCFunction)view_new_command_link, METH_VARARGS,
	 "Create new linked command in this view"
	},
#endif
	{"find_command", (PyCFunction)view_find_command, METH_VARARGS,
	 "Find named command, second argument controls inheritance"
	},
	{NULL}  /* Sentinel */
};

static PyObject * view_new(PyTypeObject * subtype,
			   PyObject * args,
			   PyObject * kwds)
{
	const char *name = NULL;
	if (PyArg_ParseTuple(args, "s", &name)) {
		clish_view_t * view;
		view = clish_view_new(name);
		return clish_python_wrap_view(view);
	}
	return NULL;
}

PyTypeObject clish_python_type_view = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.View",
	.tp_doc = "clish view",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_view_t),
	.tp_new = view_new,
	.tp_getset = view_getset,
	.tp_methods = view_methods,
};
