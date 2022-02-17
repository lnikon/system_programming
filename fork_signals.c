#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/wait.h>

/**
 * Fork of the process inherits signal handlers of its parent.
 * Pending signals are not inherited, because signal is always
 * sent for a specific pid.
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

	int childPid = fork();
	if (childPid == -1)
	{
		fprintf(stderr, "Unable to fork!\n");
		exit(EXIT_SUCCESS);
	}
	else if (childPid == 0)
	{
		for (;;)
		{
			pause();
		}
	}
	else
	{
		int wstatus;
		if (waitpid(childPid, &wstatus, WUNTRACED | WCONTINUED) == -1)
		{
			fprintf(stderr, "waitpid\n");
			exit(EXIT_FAILURE);
		}
	}

	return 0;
}

