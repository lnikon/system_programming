#include <sys/stat.h>
#include <sys/types.h>
#include <fcntl.h>
#include <ctype.h>
#include <string.h>
#include <stdio.h>
#include <unistd.h>

#ifndef BUF_SIZE
#define BUF_SIZE 1024
#endif

int main(int argc, char* argv[]) {
	int inputFd, outputFd, openFlags;
	mode_t filePerms;
	ssize_t numRead;
	char buf[BUF_SIZE];

	if (argc != 3 || strcmp(argv[1], "--help") == 0) {
		fprintf(stderr, "%s old-file new-file\n", argv[0]);
		return 1;
	}

	inputFd = open(argv[1], O_RDONLY);
	if (inputFd == -1) {
		fprintf(stderr, "error opening file %s\n", argv[1]);
		return 1;
	}

	openFlags = O_CREAT | O_WRONLY | O_TRUNC;
	filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP;
	outputFd = open(argv[2], openFlags, filePerms);
	if (outputFd == -1) {
		fprintf(stderr, "error opening file %s\n", argv[2]);
		return 1;
	}

	while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0) {
		if (write(outputFd, buf, numRead) != numRead) {
			fprintf(stderr, "could not write whole buffer\n");
			return 1;
		}
	}

	if (numRead == -1) {
		fprintf(stderr, "read -1 bytes\n");
		return 1;
	}

	if (close(inputFd) == -1) {
		fprintf(stderr, "error close input\n");
	}

	if (close(outputFd) == -1) {
		fprintf(stderr, "error close output\n");
	}

	return 0;
}
