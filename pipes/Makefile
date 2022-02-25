CC = gcc
CFLAGS = -Wall -pedantic -g -O0
RM = rm

all: basics basics_with_sighandling pipe_ls_wc popen_glob

basics: basics.c
		$(CC) $(CFLAGS) basics.c -o basics

basics_with_sighandling: basics_with_sighandling.c
		$(CC) $(CFLAGS) basics_with_sighandling.c -o basics_with_sighandling

pipe_ls_wc: pipe_ls_wc.c
		$(CC) $(CFLAGS) pipe_ls_wc.c -o pipe_ls_wc

popen_glob: popen_glob.c print_wait_status.c
		$(CC) $(CFLAGS) popen_glob.c print_wait_status.c -o popen_glob

clean:
	$(RM) basics basics_with_sighandling
