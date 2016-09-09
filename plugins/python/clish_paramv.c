
#include <clish/param.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_paramv_t *paramv;
} python_paramv_t;

#define PARAMV_TYPE (&clish_python_type_paramv)

PyTypeObject clish_python_type_paramv = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Paramv",            /*tp_name*/
    sizeof(python_paramv_t),   /*tp_basicsize*/
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
    "clish paramv",            /*tp_doc*/
};

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

static PySequenceMethods paramv_sq_methods = {
    (lenfunc)paramv_sq_length,    /*sq_length*/
    0,                            /*sq_concat*/
    0,                            /*sq_repeat*/
    (ssizeargfunc)paramv_sq_item, /*sq_item*/
    0,                            /*sq_slice*/
    0,                            /*sq_ass_item*/
    0,                            /*sq_ass_slice*/
    0,                            /*sq_contains*/
    0,                            /*sq_inplace_concat*/
    0,                            /*sq_inplace_repeat*/
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

void clish_python_init_paramv(PyObject *module)
{
	PyTypeObject *type = PARAMV_TYPE;
	/* set type fields */
	type->tp_as_sequence = &paramv_sq_methods;
	type->tp_methods = paramv_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "Paramv", (PyObject*)type);
}

PyObject * clish_python_wrap_paramv(const clish_paramv_t *paramv)
{
	if (paramv) {
		PyTypeObject *type = PARAMV_TYPE;
		python_paramv_t *self;
		/* create instance */
		self = (python_paramv_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->paramv = paramv;
		/* check sequence */
		PySequence_Check((PyObject*)self);
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_paramv_t * clish_python_unwrap_paramv(PyObject *paramv)
{
	assert(PyObject_IsInstance(paramv, PARAMV_TYPE));
	python_paramv_t *self = (python_paramv_t*)paramv;
	return self->paramv;
}
