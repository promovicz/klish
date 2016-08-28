/*
 * sym_script.c
 *
 * Function to execute a shell script.
 */

#include "private.h"
#include "lub/string.h"
#include "konf/buf.h"

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <signal.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <limits.h>

/*--------------------------------------------------------- */
CLISH_PLUGIN_OSYM(clish_script)
{
	clish_shell_t *this = clish_context__get_shell(clish_context);
	const clish_action_t *action = clish_context__get_action(clish_context);
	const char *shebang = NULL;
	const char *pager_command = NULL;
	bool_t use_pager = BOOL_FALSE;
	pid_t cpid = -1;
	int res;
	char fifo_name[PATH_MAX];
	FILE *wpipe;
	char *command = NULL;

	assert(this);
	if (!script) /* Nothing to do */
		return 0;

	/* Check if we want to enable the pager */
	pager_command = clish_shell__get_pager_command(this);
	if(pager_command) {
	  use_pager = clish_context__get_pager(clish_context);
	}
	/* Do not enable pager for interactive commands */
	if(clish_action__get_interactive(action)) {
		use_pager = BOOL_FALSE;
	}
	/* Be safe and check what we figured out */
	assert(pager_command || !use_pager);

	/* Find out shebang */
	if (action)
		shebang = clish_action__get_shebang(action);
	if (!shebang)
		shebang = clish_shell__get_default_shebang(this);
	assert(shebang);

#ifdef DEBUG
	fprintf(stderr, "SHEBANG: #!%s\n", shebang);
	fprintf(stderr, "SCRIPT: %s\n", script);
#endif /* DEBUG */

	/* Create FIFO */
	if (! clish_shell_mkfifo(this, fifo_name, sizeof(fifo_name))) {
		fprintf(stderr, "Error: Can't create temporary FIFO.\n"
			"Error: The ACTION will be not executed.\n");
		return -1;
	}

	/* Create process to write to FIFO */
	cpid = fork();
	if (cpid == -1) {
		fprintf(stderr, "Error: Can't fork the write process.\n"
			"Error: The ACTION will be not executed.\n");
		clish_shell_rmfifo(this, fifo_name);
		return -1;
	}

	/* Child: write to FIFO */
	if (cpid == 0) {
		wpipe = fopen(fifo_name, "w");
		if (!wpipe)
			_exit(-1);
		fwrite(script, strlen(script), 1, wpipe);
		fclose(wpipe);
		_exit(0);
	}

	/* Parent */
	/* Prepare command */
	lub_string_cat(&command, shebang);
	lub_string_cat(&command, " ");
	lub_string_cat(&command, fifo_name);
	if (use_pager) {
		lub_string_cat(&command, " | ( exec ");
		lub_string_cat(&command, pager_command);
		lub_string_cat(&command, " )");
	}

#ifdef DEBUG
	fprintf(stderr, "COMMAND:\n%s\n", command);
#endif /* DEBUG */

	res = system(command);

	/* Wait for the writing process */
	kill(cpid, SIGTERM);
	while (waitpid(cpid, NULL, 0) != cpid);

	/* Clean up */
	lub_string_free(command);
	clish_shell_rmfifo(this, fifo_name);

#ifdef DEBUG
	fprintf(stderr, "RETCODE: %d\n", WEXITSTATUS(res));
#endif /* DEBUG */
	return WEXITSTATUS(res);
}
