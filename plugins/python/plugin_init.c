
#include <dlfcn.h>
#include <stdio.h>

#include "lub/ini.h"
#include "lub/string.h"

#include "private.h"

#include "pyerrors.h"

/* XXX this should contain the SOVERSION */
#define PYTHON_DYNLIB "lib"PYTHON_LIB".so"

CLISH_PLUGIN_INIT(python)
{
	char *config;

#ifdef DEBUG
	fprintf(stderr, "plugin_python_dlopen(%s)\n", PYTHON_DYNLIB);
#endif
	void *handle = dlopen(PYTHON_DYNLIB, RTLD_NOW|RTLD_GLOBAL);
	if(!handle)
		return (-1);

#ifdef DEBUG
	fprintf(stderr, "plugin_python_init\n");
#endif
	if(clish_python_init(clish_shell))
		return (-1);

#ifdef DEBUG
	fprintf(stderr, "plugin_python_conf\n");
#endif
	config = clish_plugin__get_conf(plugin);
	if (config) {
		PyRun_SimpleString(config);
	}

#ifdef DEBUG
	fprintf(stderr, "plugin_python_register\n");
#endif
	clish_plugin_add_fini(plugin, clish_plugin_python_fini);
	clish_plugin_add_sym(plugin, clish_plugin_python_action, "python");

	return 0;
}
