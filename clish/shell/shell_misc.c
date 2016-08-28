/*
 * shell_misc.c
 */

#include <stdlib.h>
#include <assert.h>

#include "private.h"

CLISH_GET_STR(shell, overview);

bool_t clish_shell_is_machine_interface(const clish_shell_t *shell)
{
	assert(shell);
	if (!shell)
		return BOOL_FALSE;

	return shell->machine_interface;
}

void clish_shell_set_machine_interface(clish_shell_t *shell)
{
	assert(shell);
	if (!shell)
		return;

	shell->machine_interface = BOOL_TRUE;
	if (shell->tinyrl)
		tinyrl_set_machine_interface(shell->tinyrl);
}


void clish_shell_set_human_interface(clish_shell_t *shell)
{
	assert(shell);
	if (!shell)
		return;

	shell->machine_interface = BOOL_FALSE;
	if (shell->tinyrl)
		tinyrl_set_human_interface(shell->tinyrl);
}

const char *clish_shell__get_pager_command(clish_shell_t * instance)
{
	return getenv("CLISH_PAGER");
}

