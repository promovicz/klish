/*
 * extension.c - Python extension for loading clish
 *
 * This file is compiled into a python extension that
 * is used to link in and initialize the binding library.
 */

#include "../binding/clish_python.h"

#ifdef PYTHON3
/* definitions for python 3 */
#define ERROR return NULL
PyObject *
PyInit_clish(void)
#else
/* definitions for python 2 */
#define ERROR return
PyMODINIT_FUNC initclish(void);
PyMODINIT_FUNC initclish(void)
#endif
{
	PyObject *module;

	/* initialize bindings */
	module = clish_python_init();
	if(!module) {
		ERROR;
	}

	/* we are done */
#ifdef PYTHON3
	return module;
#endif
}
