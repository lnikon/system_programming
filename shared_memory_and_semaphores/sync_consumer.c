#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>
#include <semaphore.h>

/**
 * TODO: Create mutex struct based on POSIX semaphore with relevant functions.
 */

void errorExit(const char* msg);

int main()
{
	// sprintf error message here and pass to errorExit
	const int err_msg_size = 4096; 
	char err_msg[err_msg_size];

	// Prepare semaphore
	const char* sem_name = "pcdemo";
	sem_t* sem = NULL;
	int sem_init_value = 0;

	// Prepare shared memory
	const char* shm_name = "pcdemo";
	const int shm_size = 4096;
	const char* shm_msg_1 = "Hello\n";
	int shm_fd;
	char* ptr;

	shm_fd = shm_open(shm_name, O_CREAT | O_RDWR, 0666);
	if (shm_fd == -1)
	{
		snprintf(err_msg, err_msg_size, "unable to open shared memory segment \"%s\"\n", shm_name);
		errorExit(err_msg);
	}

	sem = sem_open(sem_name, 0, S_IRUSR | S_IWUSR, sem_init_value);
	if (sem == SEM_FAILED)
	{
		snprintf(err_msg, err_msg_size, "unable to open semaphore \"%s\"\n", sem_name);
		errorExit(err_msg);
	}

	if (sem_wait(sem) == -1)
	{
		snprintf(err_msg, err_msg_size, "unable to wait semaphore \"%s\"\n", sem_name);
		errorExit(err_msg);
	}

	ptr = (char*)mmap(0, shm_size, PROT_READ | PROT_WRITE, MAP_SHARED, shm_fd, 0);
	printf("message: %s\n", ptr);
	printf("message: %s\n", ptr + strlen(shm_msg_1));

	exit(EXIT_SUCCESS);
}

void errorExit(const char* msg)
{
		fprintf(stderr, "%s\n", msg);
		exit(EXIT_FAILURE);
}

