#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <fcntl.h>
#include <sys/shm.h>
#include <sys/stat.h>
#include <sys/mman.h>
#include <unistd.h>

int main()
{
	const int SIZE = 4096;
	const char* name = "OS";
	const char* message0 = "Hello";
	const char* message1 = "SharedMemory!";

	int fd;
	char* ptr;

	fd = shm_open(name, O_CREAT | O_RDWR, 0666);
	if (fd == -1)
	{
		fprintf(stderr, "shm_open failed");
		exit(EXIT_FAILURE);
	}

	ftruncate(fd, SIZE);
	ptr = (char*)mmap(0, SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);

	sprintf(ptr, "%s", message0);
	ptr += strlen(message0);
	sprintf(ptr, "%s", message1);
	ptr += strlen(message1);

	return 0;
}
