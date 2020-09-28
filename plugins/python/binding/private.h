
#include "clish_python.h"

#define CLISH_PYTHON_GET(clsname, typename, propname)			\
	static PyObject *						\
	clsname ## _get_ ## propname					\
	(python_ ## clsname ## _t * self, void * closure ) {		\
		return clish_python_wrap_ ## typename			\
			(clish_ ## clsname ## __get_ ## propname	\
			 (self->clsname));				\
	}

#define CLISH_PYTHON_GET_BOOL(clsname, propname)			\
	static PyObject *						\
	clsname ## _get_ ## propname					\
	(python_ ## clsname ## _t * self, void * closure ) {		\
		return clish_python_wrap_bool				\
			(clish_ ## clsname ## __get_ ## propname	\
			 (self->clsname));				\
	}

#define CLISH_PYTHON_GET_STRING(clsname, propname)			\
	static PyObject *						\
	clsname ## _get_ ## propname					\
	(python_ ## clsname ## _t * self, void * closure ) {		\
		return clish_python_wrap_string				\
			(clish_ ## clsname ## __get_ ## propname	\
			 (self->clsname));				\
	}

#define CLISH_PYTHON_WRAP(clsname)					\
	PyObject * clish_python_wrap_ ## clsname			\
	(const clish_ ## clsname ## _t * native) {			\
		if(native) {						\
			PyTypeObject *type = &clish_python_type_ ## clsname; \
			python_ ## clsname ## _t *self;			\
			self = (python_ ## clsname ## _t *)		\
				PyType_GenericNew(type, Py_None, Py_None); \
			self->clsname = (clish_ ## clsname ## _t *)native; \
			return (PyObject*)self;				\
		} else {						\
			return Py_None;					\
		}							\
	}

#define CLISH_PYTHON_UNWRAP(clsname)					\
	clish_ ## clsname ## _t * clish_python_unwrap_ ## clsname	\
	(PyObject *object) {						\
		assert(PyObject_IsInstance(object, &clish_python_type_ ## clsname)); \
		python_ ## clsname ## _t * self = (python_ ## clsname ## _t *) object; \
		return self->clsname;					\
	}
