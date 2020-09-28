
#include <clish/action.h>

#include "private.h"

typedef struct {
	PyObject_HEAD
	clish_action_t *action;
} python_action_t;

CLISH_PYTHON_WRAP(action);
CLISH_PYTHON_UNWRAP(action);

CLISH_PYTHON_GET(action, sym, builtin);
CLISH_PYTHON_GET_STRING(action, script);
CLISH_PYTHON_GET_STRING(action, shebang);
CLISH_PYTHON_GET_BOOL(action, interactive);
CLISH_PYTHON_GET_BOOL(action, permanent);
CLISH_PYTHON_GET_BOOL(action, expand);
CLISH_PYTHON_GET_BOOL(action, interrupt);
CLISH_PYTHON_GET_BOOL(action, lock);

static PyGetSetDef action_getset[] = {
	{"builtin", (getter)action_get_builtin, NULL,
	 "Builtin symbol of the action"
	},
	{"script", (getter)action_get_script, NULL,
	 "Script of the action"
	},
	{"shebang", (getter)action_get_shebang, NULL,
	 "Script shebang of the action"
	},
	{"interactive", (getter)action_get_interactive, NULL,
	 "Must be run interactively"
	},
	{"permanent", (getter)action_get_permanent, NULL,
	 "May be executed in dry-run"
	},
	{"expand", (getter)action_get_expand, NULL,
	 "Force variable expansion"
	},
	{"interrupt", (getter)action_get_interrupt, NULL,
	 "May be interrupted"
	},
	{"lock", (getter)action_get_lock, NULL,
	 "Use the command lock"
	},
	{NULL}  /* Sentinel */
};

PyTypeObject clish_python_type_action = {
	PyObject_HEAD_INIT(NULL)
	.tp_name = "clish.Action",
	.tp_doc = "clish action",
	.tp_flags = Py_TPFLAGS_DEFAULT,
	.tp_basicsize = sizeof(python_action_t),
	.tp_getset = action_getset,
};
