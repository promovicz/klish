
#include <clish/ptype.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_ptype_t *ptype;
} python_ptype_t;

#define PTYPE_TYPE (&clish_python_type_ptype)

CLISH_PYTHON_WRAP(ptype);
CLISH_PYTHON_UNWRAP(ptype);

CLISH_PYTHON_GET_STRING(ptype, name);
CLISH_PYTHON_GET_STRING(ptype, text);
CLISH_PYTHON_GET_STRING(ptype, pattern);
CLISH_PYTHON_GET_STRING(ptype, range);

static PyObject *
ptype_wrap_ptype_method(clish_ptype_method_e method)
{
	return clish_python_wrap_string
		(clish_ptype_method__get_name(method));
}

static clish_ptype_method_e
ptype_unwrap_ptype_method(PyObject *string)
{
	return clish_ptype_method_resolve
		(clish_python_unwrap_string(string));
}

static PyObject *
ptype_wrap_ptype_preprocess(clish_ptype_preprocess_e preprocess)
{
	return clish_python_wrap_string
		(clish_ptype_preprocess__get_name(preprocess));
}

static clish_ptype_preprocess_e
ptype_unwrap_ptype_preprocess(PyObject *string)
{
	return clish_ptype_preprocess_resolve
		(clish_python_unwrap_string(string));
}

static PyObject *
ptype_get_method(python_ptype_t * self, void * closure)
{
	return ptype_wrap_ptype_method
		(clish_ptype__get_method(self->ptype));
}

static PyObject *
ptype_get_preprocess(python_ptype_t * self, void * closure)
{
	return ptype_wrap_ptype_preprocess
		(clish_ptype__get_preprocess(self->ptype));
}

static PyGetSetDef ptype_getset[] = {
	{"name", (getter)ptype_get_name, NULL,
	 "Name of the ptype"
	},
	{"text", (getter)ptype_get_text, NULL,
	 "Description text of the ptype"
	},
	{"pattern", (getter)ptype_get_pattern, NULL,
	 "Pattern definition of the ptype"
	},
	{"range", (getter)ptype_get_range, NULL,
	 "Range definition of the ptype"
	},
	{"method", (getter)ptype_get_method, NULL,
	 "Pattern method of the ptype"
	},
	{"preprocess", (getter)ptype_get_preprocess, NULL,
	 "Preprocessing method of the ptype"
	},
	{NULL}  /* Sentinel */
};

static PyObject *
ptype_set_pattern(python_ptype_t * self, PyObject * args)
{
	const char *pattern;
	const char *method;
	if (PyArg_ParseTuple(args, "ss", &pattern, &method)) {
		clish_ptype__set_pattern
			(self->ptype,
			 pattern,
			 clish_ptype_method_resolve(method));
	}
	return Py_None;
}

static PyMethodDef ptype_methods[] = {
	{"set_pattern", (PyCFunction)ptype_set_pattern, METH_VARARGS,
	 "Set ptype method and pattern"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_ptype = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.PType",
	.tp_doc = "clish ptype",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_ptype_t),
	.tp_getset = ptype_getset,
	.tp_methods = ptype_methods,
};
