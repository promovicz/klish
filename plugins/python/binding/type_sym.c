
#include <clish/plugin.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_sym_t *sym;
} python_sym_t;

#define SYM_TYPE (&clish_python_type_sym)

CLISH_PYTHON_WRAP(sym);
CLISH_PYTHON_UNWRAP(sym);

CLISH_PYTHON_GET_STRING(sym, name);
CLISH_PYTHON_GET_BOOL(sym, permanent);

static PyGetSetDef sym_getset[] = {
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
#if 0
	/* Probably never */
	{"func", (getter)sym_get_func, NULL,
	 "Function of the symbol"
	},
#endif
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_sym = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Sym",
	.tp_doc = "clish symbol",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_sym_t),
	.tp_getset = sym_getset,
};
