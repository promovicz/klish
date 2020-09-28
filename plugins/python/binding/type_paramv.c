
#include <clish/param.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_paramv_t *paramv;
} python_paramv_t;

#define PARAMV_TYPE (&clish_python_type_paramv)

CLISH_PYTHON_WRAP(paramv);
CLISH_PYTHON_UNWRAP(paramv);

static Py_ssize_t paramv_sq_length(python_paramv_t * self)
{
	return clish_paramv__get_count(self->paramv);
}

static PyObject * paramv_sq_item(python_paramv_t * self, Py_ssize_t index)
{
	if (index >= 0 && index < paramv_sq_length(self)) {
		return clish_python_wrap_param
			(clish_paramv__get_param(self->paramv, index));
	} else {
		return NULL;
	}
}

static PySequenceMethods paramv_sequence = {
	.sq_length = (lenfunc)paramv_sq_length,
	.sq_item = (ssizeargfunc)paramv_sq_item,
};

static PyMethodDef paramv_methods[] = {
#if 0
	{"find_param", (PyCFunction)paramv_find_param, METH_NOARGS,
	 "Find named parameter"
	},
	{"find_default", (PyCFunction)paramv_find_default, METH_NOARGS,
	 "Find default parameter"
	},
	{"insert", (PyCFunction)paramv_insert, METH_NOARGS,
	 "Insert a parameter"
	},
	{"remove", (PyCFunction)paramv_remove, METH_NOARGS,
	 "Remove a parameter"
	},
#endif
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_paramv = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Paramv",
	.tp_doc = "clish paramv",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_paramv_t),
	.tp_methods = paramv_methods,
	.tp_as_sequence = &paramv_sequence,
};
