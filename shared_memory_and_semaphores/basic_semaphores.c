#include <semaphore.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <stdio.h>

/**
 * Semaphores come in two flavors:
 * Named semaphore - any unrelated process can access this semaphore.
 *                    its name should start with '/' and be null-terminated.
 * Unnamed semaphore - stored in thread-shared or process-shared memory in agreed-upon location. 
 */

void errorExit(const char* msg);

int main()
{
	int value = 1;
	sem_t* sem;
	const char* name = "/demo1";

	sem = sem_open(name, O_CREAT, S_IRUSR | S_IWUSR, value);
  if (sem == SEM_FAILED)
	{
		errorExit("unable to open semaphore");
	}

	if (sem_post(sem) == -1)
	{
		errorExit("unable to wait on semaphore");
	}

	if (sem_close(sem) == -1)
	{
		errorExit("unable to close semaphore");
	}

	if (sem_unlink(name) == -1)
	{
		errorExit("unable to unlink semaphore");
	}

	exit(EXIT_SUCCESS);
}

void errorExit(const char* msg)
{
		fprintf(stderr, "%s\n", msg);
		exit(EXIT_FAILURE);
}
