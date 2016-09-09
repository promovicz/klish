
#include <clish/param.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_param_t *param;
} python_param_t;

#define PARAM_TYPE (&clish_python_type_param)

PyTypeObject clish_python_type_param = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Param",             /*tp_name*/
    sizeof(python_param_t),    /*tp_basicsize*/
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
    "clish param",             /*tp_doc*/
};

static PyObject *
param_get_name(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_name(self->param));
}

static PyObject *
param_get_ptype_name(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_ptype_name(self->param));
}

static PyObject *
param_get_text(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_text(self->param));
}

static PyObject *
param_get_range(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_range(self->param));
}

static PyObject *
param_get_default(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_default(self->param));
}

static PyObject *
param_get_ptype(python_param_t * self, void * closure)
{
	return clish_python_wrap_ptype
		(clish_param__get_ptype(self->param));
}

static PyObject *
param_get_optional(python_param_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_param__get_optional(self->param));
}

static PyObject *
param_get_order(python_param_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_param__get_order(self->param));
}

static PyObject *
param_get_value(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_value(self->param));
}

static PyObject *
param_get_hidden(python_param_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_param__get_hidden(self->param));
}

static PyObject *
param_get_params(python_param_t * self, void * closure)
{
	return clish_python_wrap_paramv
		(clish_param__get_paramv(self->param));
}

static PyObject *
param_get_test(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_test(self->param));
}

static PyObject *
param_get_completion(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_completion(self->param));
}

static PyObject *
param_get_access(python_param_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_param__get_access(self->param));
}

static PyGetSetDef param_getset[] = {
	{"name", (getter)param_get_name, NULL,
     "Return parameter name"
    },
	{"ptype_name", (getter)param_get_ptype_name, NULL,
     "Return parameter ptype name"
    },
	{"text", (getter)param_get_text, NULL,
     "Return parameter text"
    },
	{"range", (getter)param_get_range, NULL,
     "Return parameter range"
    },
	{"default", (getter)param_get_default, NULL,
     "Return parameter default"
    },
	{"ptype", (getter)param_get_ptype, NULL,
     "Return parameter ptype"
    },
#if 0
	{"mode", (getter)param_get_mode, NULL,
     "Return parameter mode"
    },
#endif
	{"params", (getter)param_get_params, NULL,
     "Return parameter params"
    },
	{"optional", (getter)param_get_optional, NULL,
     "Return parameter optional"
    },
	{"order", (getter)param_get_order, NULL,
     "Return parameter order"
    },
	{"value", (getter)param_get_value, NULL,
     "Return parameter value"
    },
	{"hidden", (getter)param_get_hidden, NULL,
     "Return parameter hidden"
    },
	{"test", (getter)param_get_test, NULL,
     "Return parameter hidden"
    },
	{"completion", (getter)param_get_completion, NULL,
     "Return parameter hidden"
    },
	{"access", (getter)param_get_access, NULL,
     "Return parameter hidden"
    },
    {NULL}  /* Sentinel */
};

static PyMethodDef param_methods[] = {
#if 0
	{"find_param", (PyCFunction)context_find_param, METH_NOARGS,
     "Find named parameter"
	},
#endif
    {NULL}  /* Sentinel */
};

void clish_python_init_param(PyObject *module)
{
	PyTypeObject *type = PARAM_TYPE;
	/* set type fields */
	type->tp_getset = param_getset;
	type->tp_methods = param_methods;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "Param", (PyObject*)type);
}

PyObject * clish_python_wrap_param(const clish_param_t *param)
{
	if (param) {
		PyTypeObject *type = PARAM_TYPE;
		python_param_t *self;
		/* create instance */
		self = (python_param_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->param = param;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_param_t * clish_python_unwrap_param(PyObject *param)
{
	assert(PyObject_IsInstance(param, PARAM_TYPE));
	python_param_t *self = (python_param_t*)param;
	return self->param;
}
