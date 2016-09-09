
#include <clish/pargv.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_pargv_t *pargv;
} python_pargv_t;

#define PARGV_TYPE (&clish_python_type_pargv)

PyTypeObject clish_python_type_pargv = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Pargv",             /*tp_name*/
    sizeof(python_pargv_t),    /*tp_basicsize*/
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
    "clish pargv",             /*tp_doc*/
};

static Py_ssize_t pargv_mp_length(python_pargv_t * self)
{
	return clish_pargv__get_count(self->pargv);
}

static PyObject * pargv_mp_subscript(python_pargv_t * self, PyObject * key)
{
	PyObject * res = NULL;
	char *name = PyString_AsString(key);
	const clish_parg_t * parg = clish_pargv_find_arg(self->pargv, name);
	if (parg) {
		res = clish_python_wrap_string
			(clish_parg__get_value(parg));
	} else {
		res = Py_None;
	}
	return res;
}

static PyMappingMethods pargv_mp_methods = {
    (lenfunc)pargv_mp_length,              /*mp_length*/
    (binaryfunc)pargv_mp_subscript,        /*mp_subscript*/
	NULL,                                  /*mp_ass_subscript*/
};

static PyMethodDef pargv_methods[] = {
#if 0
	{"find_param", (PyCFunction)pargv_find_param, METH_NOARGS,
     "Find named parameter"
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

void clish_python_init_pargv(PyObject *module)
{
	PyTypeObject *type = PARGV_TYPE;
	/* set type fields */
	type->tp_as_mapping = &pargv_mp_methods;
	type->tp_methods = pargv_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "Pargv", (PyObject*)type);
}

PyObject * clish_python_wrap_pargv(const clish_pargv_t *pargv)
{
	if (pargv) {
		PyTypeObject *type = PARGV_TYPE;
		python_pargv_t *self;
		/* create instance */
		self = (python_pargv_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->pargv = pargv;
		/* check sequence */
		PySequence_Check((PyObject*)self);
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_pargv_t * clish_python_unwrap_pargv(PyObject *pargv)
{
	assert(PyObject_IsInstance(pargv, PARGV_TYPE));
	python_pargv_t *self = (python_pargv_t*)pargv;
	return self->pargv;
}
