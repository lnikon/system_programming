#include <sys/epoll.h>

int main() {
	int efd = epoll_create1(EPOLL_CLOEXEC);

	return 0;
}
