#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * When process created via one of exec() family syscalls,
 * then all signals are set to their default handlers unless
 * parent process ignores them, because freshly executed process
 * doesn't shares address space with the parent.
 * Pending signals are inherited.
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

	char* args[] = {"./infinite_pause", NULL};
	int ec = execvp(args[0], args);
	if (ec == -1)
	{
		fprintf(stderr, "Unable to execvp!\n");
		exit(EXIT_FAILURE);
	}

	return 0;
}

