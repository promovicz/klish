
#include <clish/shell.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_context_t *context;
} python_vars_t;

#define VARS_TYPE (&clish_python_type_vars)

static PyObject * vars_mp_subscript(python_vars_t * self, PyObject * key)
{
	const char *name = clish_python_unwrap_string(key);
	const char *value = clish_shell_expand_var(name, self->context);
	return clish_python_wrap_string(value);
}

static PyMappingMethods vars_mapping = {
	.mp_subscript = (binaryfunc)vars_mp_subscript,
};

static PyMethodDef vars_methods[] = {
	{NULL}  /* Sentinel */
};

PyObject * clish_python_wrap_as_vars(const clish_context_t *context)
{
	if (context) {
		PyTypeObject *type = VARS_TYPE;
		python_vars_t *self;
		/* create instance */
		self = (python_vars_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->context = (clish_context_t *)context;
		/* check sequence */
		PySequence_Check((PyObject*)self);
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

PyTypeObject clish_python_type_vars = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Vars",
	.tp_doc = "clish vars",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_vars_t),
	.tp_methods = vars_methods,
	.tp_as_mapping = &vars_mapping,
};
