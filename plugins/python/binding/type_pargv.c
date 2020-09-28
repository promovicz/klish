
#include <clish/pargv.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_pargv_t *pargv;
} python_pargv_t;

#define PARGV_TYPE (&clish_python_type_pargv)

CLISH_PYTHON_WRAP(pargv);
CLISH_PYTHON_UNWRAP(pargv);

static Py_ssize_t pargv_mp_length(python_pargv_t * self)
{
	return clish_pargv__get_count(self->pargv);
}

static PyObject * pargv_mp_subscript(python_pargv_t * self, PyObject * key)
{
	const char *name = clish_python_unwrap_string(key);
	const clish_parg_t *parg = clish_pargv_find_arg(self->pargv, name);
	if(parg) {
		const char *value = clish_parg__get_value(parg);
		return clish_python_wrap_string(value);
	} else {
		return Py_None;
	}
}

static PyMappingMethods pargv_mapping = {
	.mp_length = (lenfunc)pargv_mp_length,
	.mp_subscript = (binaryfunc)pargv_mp_subscript,
};

static PyObject *
pargv_as_dict(python_pargv_t * self)
{
	unsigned u, n;
	clish_pargv_t *pargv = self->pargv;
	PyObject *dict = PyDict_New();

	n = clish_pargv__get_count(pargv);
	for(u = 0; u < n; u++) {
		clish_parg_t *parg = clish_pargv__get_parg(pargv, u);
		PyObject *name = clish_python_wrap_string(clish_parg__get_name(parg));
		PyObject *value = clish_python_wrap_string(clish_parg__get_value(parg));
		PyDict_SetItem(dict, name, value);
	}

	return dict;
}

static PyMethodDef pargv_methods[] = {
	{"as_dict", (PyCFunction)pargv_as_dict, METH_NOARGS,
	 "Convert to python dict"
	},
#if 0
	{"find_param", (PyCFunction)pargv_find_param, METH_NOARGS,
	 "Find named parameter"
	},
	{"find_value", (PyCFunction)pargv_find_value, METH_NOARGS,
	 "Find parameter value"
	},
	{"find_default", (PyCFunction)pargv_find_default, METH_NOARGS,
	 "Find default parameter"
	},
	{"insert", (PyCFunction)pargv_insert, METH_NOARGS,
	 "Insert a parameter"
	},
	{"remove", (PyCFunction)pargv_remove, METH_NOARGS,
	 "Remove a parameter"
	},
#endif
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_pargv = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Pargv",
	.tp_doc = "clish pargv",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_pargv_t),
	.tp_methods = pargv_methods,
	.tp_as_mapping = &pargv_mapping,
};
