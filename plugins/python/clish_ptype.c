
#include <clish/ptype.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_ptype_t *ptype;
} python_ptype_t;

#define PTYPE_TYPE (&clish_python_type_ptype)

PyTypeObject clish_python_type_ptype = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.PType",             /*tp_name*/
    sizeof(python_ptype_t),    /*tp_basicsize*/
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
    "clish ptype",             /*tp_doc*/
};

static PyObject *
ptype_wrap_method(clish_ptype_method_e method)
{
	return clish_python_wrap_string
		(clish_ptype_method__get_name(method));
}

static PyObject *
ptype_wrap_preprocess(clish_ptype_preprocess_e preprocess)
{
	return clish_python_wrap_string
		(clish_ptype_preprocess__get_name(preprocess));
}

static clish_ptype_method_e
ptype_unwrap_method(const char * method)
{
	return clish_ptype_method_resolve(method);
}

static clish_ptype_preprocess_e
ptype_unwrap_preprocess(const char * preprocess)
{
	return clish_ptype_preprocess_resolve(preprocess);
}

static PyObject *
ptype_get_name(python_ptype_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_ptype__get_name(self->ptype));
}

static PyObject *
ptype_get_text(python_ptype_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_ptype__get_text(self->ptype));
}

static PyObject *
ptype_get_pattern(python_ptype_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_ptype__get_pattern(self->ptype));
}

static PyObject *
ptype_get_range(python_ptype_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_ptype__get_range(self->ptype));
}

static PyObject *
ptype_get_method(python_ptype_t * self, void * closure)
{
	return ptype_wrap_method
		(clish_ptype__get_method(self->ptype));
}

static PyObject *
ptype_get_preprocess(python_ptype_t * self, void * closure)
{
	return ptype_wrap_preprocess
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
			 ptype_unwrap_method(method));
	}
	return Py_None;
}

static PyMethodDef ptype_methods[] = {
	{"set_pattern", (PyCFunction)ptype_set_pattern, METH_VARARGS,
     "Set ptype method and pattern"
	},
	{NULL}  /* Sentinel */
};

void clish_python_init_ptype(PyObject *module)
{
	PyTypeObject *type = PTYPE_TYPE;
	/* set type fields */
	type->tp_getset = ptype_getset;
	type->tp_methods = ptype_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "PType", (PyObject*)type);
}

PyObject * clish_python_wrap_ptype(const clish_ptype_t *ptype)
{
	if (ptype) {
		PyTypeObject *type = PTYPE_TYPE;
		python_ptype_t *self;
		/* create instance */
		self = (python_ptype_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->ptype = ptype;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_ptype_t * clish_python_unwrap_ptype(PyObject *ptype)
{
	assert(PyObject_IsInstance(ptype, PTYPE_TYPE));
	python_ptype_t *self = (python_ptype_t*)ptype;
	return self->ptype;
}
