#include <signal.h>
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <sys/types.h>

/**
 * To kill another process kill(pid_t pid, int signo) syscall can be used.
 * If pid > 0, then kill is send to process indentified by pid.
 * If pid = 0, then kill is send to every process in the involing process's process group.
 * If pid = -1, kill to every process that current process has permission to set.
 * If pid < -1, then kill is send to process groupd -pid.
 * Returns 0 on sucess.
 * Return -1 on failure and set errno to one of:
 * - EINVAL - invalid signal,
 * - EPERM - lack of permission,
 * - ESRCH - process or process group doesn;t exist or is zombie.
 */

int main()
{
	return 0;
}

