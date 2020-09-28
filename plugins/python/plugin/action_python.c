
#include "private.h"

CLISH_PLUGIN_SYM(clish_plugin_python_action_python)
{
	/* succeed if there is no script */
	if (!script)
		return (0);

	/* execute the code */
	return clish_python_context_execute(clish_context, script);
}
