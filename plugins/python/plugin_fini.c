
#include <stdio.h>

#include "private.h"

CLISH_PLUGIN_FINI(clish_plugin_python_fini)
{
#ifdef DEBUG
	fprintf(stderr,"plugin_python_fini\n");
#endif

	if(clish_python_fini(clish_shell))
		return (-1);

	return (0);
}
