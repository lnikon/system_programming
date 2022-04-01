#pragma once

#include <sys/socket.h>
#include <netinet/in.h>

void *get_in_addr(struct sockaddr *sa);
