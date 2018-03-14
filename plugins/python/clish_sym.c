
#include <clish/plugin.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_sym_t *sym;
} python_sym_t;

#define SYM_TYPE (&clish_python_type_sym)

PyTypeObject clish_python_type_sym = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Sym",               /*tp_name*/
    sizeof(python_sym_t),      /*tp_basicsize*/
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
    "clish symbol",            /*tp_doc*/
};

static PyObject *
sym_get_name(python_sym_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_sym__get_name(self->sym));
}

static PyObject *
sym_get_permanent(python_sym_t * self, void * closure)
{
	return clish_python_wrap_bool
		(clish_sym__get_permanent(self->sym));
}

#if 0
static PyObject *
sym_get_plugin(python_sym_t * self, void * closure)
{
	return clish_python_wrap_plugin
		(clish_sym__get_plugin(self->sym));
}
#endif

static PyGetSetDef sym_getset[] = {
#if 0
	/* Probably never */
	{"func", (getter)sym_get_func, NULL,
		"Function of the symbol"
	},
#endif
	{"name", (getter)sym_get_name, NULL,
		"Name of the symbol"
	},
	{"permanent", (getter)sym_get_permanent, NULL,
		"Action of the symbol"
	},
#if 0
	{"plugin", (getter)sym_get_plugin, NULL,
		"Plugin of the symbol"
	},
	{"type", (getter)sym_get_type, NULL,
		"Type of the symbol"
	},
	{"api", (getter)sym_get_api, NULL,
		"API of the symbol"
	},
#endif
	{NULL}  /* Sentinel */
};

void clish_python_init_sym(PyObject *module)
{
	PyTypeObject *type = SYM_TYPE;
	/* set type fields */
	type->tp_getset = sym_getset;
	/* prepare type */
	if (PyType_Ready(type) < 0)
		return;
	/* add type to module */
	Py_INCREF((PyObject*)type);
	PyModule_AddObject(module, "Sym", (PyObject*)type);
}

PyObject * clish_python_wrap_sym(const clish_sym_t *sym)
{
	if (sym) {
		PyTypeObject *type = SYM_TYPE;
		python_sym_t *self;
		/* create instance */
		self = (python_sym_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->sym = sym;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_sym_t * clish_python_unwrap_sym(PyObject *sym)
{
	assert(PyObject_IsInstance(sym, SYM_TYPE));
	python_sym_t *self = (python_sym_t*)sym;
	return self->sym;
}
