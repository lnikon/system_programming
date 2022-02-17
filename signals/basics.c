#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>

/**
 * Signals are asynchronous events coming from the kernel and handled via signals handlers.
 * Signal handlers can be configured via signal() system call for each process.
 * Signal handlers are also executed asynchronously.
 * To ignore the signal SIG_IGN handler should be used.
 * To execute default handler SIG_DFL should be used.
 * When signal is received following can happen:
 *   - Ignore the signal - depending on how the specific signal handler configuration.
 *     SIGKILL and SIGSTOP NEVER can be ignored!
 *   - Catch and handle the signal - Process pauses execution of the current function
 *     and proceeds to the execution of the signal handler. When signal handler exection
 *     is finished, process continues from the suspension point. SIGKILL and SIGSTOP CAN NOT
 *     be caught.
 *   - Perform default action - Execute default default handler for that signal.
 */

static void sigint_handler(int signo)
{
	if (signo == SIGINT)
	{
		printf("Caught SIGINT!\n");
	}
	else if (signo == SIGTERM)
	{
		printf("Caught SIGTERM!\n");
	}
	else
	{
		fprintf(stderr, "Unexpected signal!\n");
		exit(EXIT_FAILURE);
	}

	exit(EXIT_SUCCESS);
}

int main()
{
	if (signal(SIGINT, sigint_handler) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGINT!\n");
		exit(EXIT_SUCCESS);
	}

	if (signal(SIGTERM, sigint_handler) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGTERM!\n");
		exit(EXIT_SUCCESS);
	}

	if (signal(SIGPROF, SIG_DFL) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGPROF!\n");
		exit(EXIT_SUCCESS);
	}

	if (signal(SIGHUP, SIG_IGN) == SIG_ERR)
	{
		fprintf(stderr, "Cannot handle SIGHUP!\n");
		exit(EXIT_SUCCESS);
	}

	for (;;)
	{
		pause();
	}

	return 0;
}

