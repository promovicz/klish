
#include <clish/param.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_param_t *param;
} python_param_t;

#define PARAM_TYPE (&clish_python_type_param)

CLISH_PYTHON_WRAP(param);
CLISH_PYTHON_UNWRAP(param);

CLISH_PYTHON_GET(param, ptype, ptype);
CLISH_PYTHON_GET(param, paramv, paramv);
CLISH_PYTHON_GET_BOOL(param, optional);
CLISH_PYTHON_GET_BOOL(param, order);
CLISH_PYTHON_GET_BOOL(param, hidden);
CLISH_PYTHON_GET_STRING(param, name);
CLISH_PYTHON_GET_STRING(param, ptype_name);
CLISH_PYTHON_GET_STRING(param, text);
CLISH_PYTHON_GET_STRING(param, range);
CLISH_PYTHON_GET_STRING(param, defval);
CLISH_PYTHON_GET_STRING(param, value);
CLISH_PYTHON_GET_STRING(param, test);
CLISH_PYTHON_GET_STRING(param, completion);
CLISH_PYTHON_GET_STRING(param, access);

static PyGetSetDef param_getset[] = {
	{"ptype", (getter)param_get_ptype, NULL,
	 "Return parameter ptype"
	},
	{"paramv", (getter)param_get_paramv, NULL,
	 "Return parameter params"
	},
	{"optional", (getter)param_get_optional, NULL,
	 "Return parameter optional"
	},
	{"order", (getter)param_get_order, NULL,
	 "Return parameter order"
	},
	{"hidden", (getter)param_get_hidden, NULL,
	 "Return parameter hidden"
	},
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
	{"defval", (getter)param_get_defval, NULL,
	 "Return parameter defval"
	},
	{"value", (getter)param_get_value, NULL,
	 "Return parameter value"
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
#if 0
	{"mode", (getter)param_get_mode, NULL,
	 "Return parameter mode"
	},
#endif
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

PyTypeObject clish_python_type_param = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Param",
	.tp_doc = "clish param",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_param_t),
	.tp_getset = param_getset,
	.tp_methods = param_methods,
};
