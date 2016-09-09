
#include <clish/action.h>

#include "private.h"

typedef struct {
    PyObject_HEAD
	clish_action_t *action;
} python_action_t;

#define ACTION_TYPE (&clish_python_type_action)

PyTypeObject clish_python_type_action = {
    PyObject_HEAD_INIT(NULL)
    0,                         /*ob_size*/
    "clish.Action",            /*tp_name*/
    sizeof(python_action_t),   /*tp_basicsize*/
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
    "clish action",            /*tp_doc*/
};

static PyObject *
action_get_script(python_action_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_action__get_script(self->action));
}

static PyObject *
action_get_builtin(python_action_t * self, void * closure)
{
	return clish_python_wrap_sym
		(clish_action__get_builtin(self->action));
}

static PyObject *
action_get_shebang(python_action_t * self, void * closure)
{
	return clish_python_wrap_string
		(clish_action__get_shebang(self->action));
}

static PyGetSetDef action_getset[] = {
	{"script", (getter)action_get_script, NULL,
     "Script of the action"
	},
	{"builtin", (getter)action_get_builtin, NULL,
     "Builtin symbol of the action"
    },
	{"shebang", (getter)action_get_shebang, NULL,
     "Script shebang of the action"
    },
    {NULL}  /* Sentinel */
};

void clish_python_init_action(PyObject *module)
{
	PyTypeObject *type = ACTION_TYPE;
	/* set type fields */
	type->tp_getset = action_getset;
	/* prepare type */
    if (PyType_Ready(type) < 0)
        return;
	/* add type to module */
    Py_INCREF((PyObject*)type);
    PyModule_AddObject(module, "Action", (PyObject*)type);
}

PyObject * clish_python_wrap_action(const clish_action_t *action)
{
	if(action) {
		PyTypeObject *type = ACTION_TYPE;
		python_action_t *self;
		/* create instance */
		self = (python_action_t*)PyType_GenericNew(type, Py_None, Py_None);
		self->action = action;
		/* return it */
		return (PyObject*)self;
	} else {
		return Py_None;
	}
}

clish_action_t * clish_python_unwrap_action(PyObject *action)
{
	assert(PyObject_IsInstance(action, ACTION_TYPE));
	python_action_t *self = (python_action_t*)action;
	return self->action;
}
