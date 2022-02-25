#include <sys/wait.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

/*
 * Pipe is a byte stream. There is no concept of random reads or messages.
 * Reading process can read blocks of bytes of any size.
 * Data passes through the pipe sequentially - bytes are read from pipe in exactly
 * the order they were written.
 *
 * Pipes are *uni*directional.
 *
 * Writes of up to PIPE_BUF bytes are guaranteed to be atomic.
 */

int main(int argc, char* argv[])
{
	if (argc != 2)
	{
		fprintf(stderr, "usage: %s <string>\n", argv[0]);
		exit(EXIT_FAILURE);
	}

	int pipefd[2];
	pid_t childPid;
	char buf;

	if (pipe(pipefd) == -1)
	{
		fprintf(stderr, "unable to open pipe\n");
		exit(EXIT_FAILURE);
	}

	childPid = fork();
	if (childPid == -1)
	{
		fprintf(stderr, "unable to fork\n");
		exit(EXIT_FAILURE);
	}
	else if (childPid == 0)
	{
		close(pipefd[1]);
		while (read(pipefd[0], &buf, 1) > 0)
		{
			write(STDOUT_FILENO, &buf, 1);
		}
		write(STDOUT_FILENO, "\n", 1);
		close(pipefd[0]);
		_exit(EXIT_SUCCESS);
	}
	else
	{
		close(pipefd[0]);
		write(pipefd[1], argv[1], strlen(argv[1]));
		close(pipefd[1]);
		wait(NULL);
		exit(EXIT_SUCCESS);
	}

	return 0;
}
