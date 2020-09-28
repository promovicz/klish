
#include <clish/shell.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_context_t *context;
} python_context_t;

#define CONTEXT_TYPE (&clish_python_type_context)

CLISH_PYTHON_WRAP(context);
CLISH_PYTHON_UNWRAP(context);

CLISH_PYTHON_GET(context, shell, shell);
CLISH_PYTHON_GET(context, action, action);
CLISH_PYTHON_GET(context, command, cmd);
CLISH_PYTHON_GET(context, pargv, pargv);

static PyObject *
context_get_view(python_context_t * self, void * closure)
{
	return clish_python_wrap_view
		(clish_shell__get_view(clish_context__get_shell(self->context)));
}

static PyGetSetDef context_getset[] = {
	{"shell", (getter)context_get_shell, NULL,
	 "The shell"
	},
	{"action", (getter)context_get_action, NULL,
	 "The current action"
	},
	{"cmd", (getter)context_get_cmd, NULL,
	 "The current command"
	},
	{"pargv", (getter)context_get_pargv, NULL,
	 "The current parameters as a pargv"
	},
	{"view", (getter)context_get_view, NULL,
	 "The current view"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_context = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Context",
	.tp_doc = "clish context",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_context_t),
	.tp_getset = context_getset,
};
