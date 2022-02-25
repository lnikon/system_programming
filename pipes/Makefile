CC = gcc
CFLAGS = -Wall -pedantic -g -O0
RM = rm

all: basics basics_with_sighandling pipe_ls_wc

basics: basics.c
		$(CC) $(CFLAGS) basics.c -o basics

basics_with_sighandling: basics_with_sighandling.c
		$(CC) $(CFLAGS) basics_with_sighandling.c -o basics_with_sighandling

pipe_ls_wc: pipe_ls_wc.c
		$(CC) $(CFLAGS) pipe_ls_wc.c -o pipe_ls_wc

clean:
	$(RM) basics basics_with_sighandling
