/*************************************************************************\
*                  Copyright (C) Michael Kerrisk, 2022.                   *
*                                                                         *
* This program is free software. You may use, modify, and redistribute it *
* under the terms of the GNU General Public License as published by the   *
* Free Software Foundation, either version 3 or (at your option) any      *
* later version. This program is distributed without any warranty.  See   *
* the file COPYING.gpl-v3 for details.                                    *
\*************************************************************************/

/* Listing 44-4 */

/* pipe_ls_wc.c

   Demonstrate the use of a pipe to connect two filters. We use fork()
   to create two children. The first one execs ls(1), which writes to
   the pipe, the second execs wc(1) to read from the pipe.
*/
#include <sys/wait.h>
#include "tlpi_hdr.h"

int
main(int argc, char *argv[])
{
    int pfd[2];                                     /* Pipe file descriptors */

    if (pipe(pfd) == -1)                            /* Create pipe */
        printf("pipe");

    switch (fork()) {
    case -1:
        printf("fork");

    case 0:             /* First child: exec 'ls' to write to pipe */
        if (close(pfd[0]) == -1)                    /* Read end is unused */
            printf("close 1");

        /* Duplicate stdout on write end of pipe; close duplicated descriptor */

        if (pfd[1] != STDOUT_FILENO) {              /* Defensive check */
            if (dup2(pfd[1], STDOUT_FILENO) == -1)
                printf("dup2 1");
            if (close(pfd[1]) == -1)
                printf("close 2");
        }

        execlp("ls", "ls", (char *) NULL);          /* Writes to pipe */
        printf("execlp ls");

    default:            /* Parent falls through to create next child */
        break;
    }

    switch (fork()) {
    case -1:
        printf("fork");

    case 0:             /* Second child: exec 'wc' to read from pipe */
        if (close(pfd[1]) == -1)                    /* Write end is unused */
            printf("close 3");

        /* Duplicate stdin on read end of pipe; close duplicated descriptor */

        if (pfd[0] != STDIN_FILENO) {               /* Defensive check */
            if (dup2(pfd[0], STDIN_FILENO) == -1)
                printf("dup2 2");
            if (close(pfd[0]) == -1)
                printf("close 4");
        }

        execlp("wc", "wc", "-l", (char *) NULL);
        printf("execlp wc");

    default: /* Parent falls through */
        break;
    }

    /* Parent closes unused file descriptors for pipe, and waits for children */

    if (close(pfd[0]) == -1)
        printf("close 5");
    if (close(pfd[1]) == -1)
        printf("close 6");
    if (wait(NULL) == -1)
        printf("wait 1");
    if (wait(NULL) == -1)
        printf("wait 2");

    exit(EXIT_SUCCESS);
}
