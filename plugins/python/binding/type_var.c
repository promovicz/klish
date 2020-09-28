
#include <clish/var.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_var_t *var;
} python_var_t;

#define VAR_TYPE (&clish_python_type_var)

CLISH_PYTHON_WRAP(var);
CLISH_PYTHON_UNWRAP(var);

CLISH_PYTHON_GET_STRING(var, name);
CLISH_PYTHON_GET_BOOL(var, dynamic);
CLISH_PYTHON_GET_STRING(var, value);
CLISH_PYTHON_GET_STRING(var, saved);
CLISH_PYTHON_GET(var, action, action);

static PyGetSetDef var_getset[] = {
	{"name", (getter)var_get_name, NULL,
	 "Name of the variable"
	},
	{"dynamic", (getter)var_get_dynamic, NULL,
	 "True if variable is dynamic"
	},
	{"value", (getter)var_get_value, NULL,
	 "Value of the variable"
	},
	{"saved", (getter)var_get_saved, NULL,
	 "Saved value of the variable"
	},
	{"action", (getter)var_get_action, NULL,
	 "Action of the variable"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_var = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Var",
	.tp_doc = "clish var",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_var_t),
	.tp_getset = var_getset,
};
