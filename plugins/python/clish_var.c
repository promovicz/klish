
#include <clish/var.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_var_t *var;
} python_var_t;

#define VAR_TYPE (&clish_python_type_var)

PyTypeObject clish_python_type_var = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Var",               /*tp_name*/
    sizeof(python_var_t),      /*tp_basicsize*/
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
    "clish var",               /*tp_doc*/
};

static PyObject *
var_get_name(python_var_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_var__get_name(self->var));
}

static PyObject *
var_get_action(python_var_t * self, void * closure)
{
	return clish_python_wrap_action
		(clish_var__get_action(self->var));
}

static PyObject *
var_get_dynamic(python_var_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_var__get_dynamic(self->var));
}

static PyObject *
var_get_saved(python_var_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_var__get_saved(self->var));
}

static PyObject *
var_get_value(python_var_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_var__get_value(self->var));
}

static PyGetSetDef var_getset[] = {
	{"name", (getter)var_get_name, NULL,
     "Name of the variable"
	},
	{"action", (getter)var_get_action, NULL,
     "Action of the variable"
    },
	{"dynamic", (getter)var_get_dynamic, NULL,
     "True if variable is dynamic"
    },
	{"saved", (getter)var_get_saved, NULL,
     "Saved value of the variable"
    },
	{"value", (getter)var_get_value, NULL,
     "Value of the variable"
    },
    {NULL}  /* Sentinel */
};

void clish_python_init_var(PyObject *module)
{
	PyTypeObject *type = VAR_TYPE;
	/* set type fields */
	type->tp_getset = var_getset;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "Var", (PyObject*)type);
}

PyObject * clish_python_wrap_var(const clish_var_t *var)
{
	if (var) {
		PyTypeObject *type = VAR_TYPE;
		python_var_t *self;
		/* create instance */
		self = (python_var_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->var = var;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_var_t * clish_python_unwrap_var(PyObject *var)
{
	assert(PyObject_IsInstance(var, VAR_TYPE));
	python_var_t *self = (python_var_t*)var;
	return self->var;
}
