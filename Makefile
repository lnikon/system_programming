CC = gcc
CFLAGS = -Wall -pedantic -g -O0
RM = rm

all: infinite_pause basics fork_signals exec_signals

infinite_pause: infinite_pause.c
		$(CC) $(CFLAGS) infinite_pause.c -o infinite_pause

basics: basics.c
		$(CC) $(CFLAGS) basics.c -o basics

fork_signals: fork_signals.c
		$(CC) $(CFLAGS) fork_signals.c -o fork_signals

exec_signals: infinite_pause exec_signals.c
		$(CC) $(CFLAGS) exec_signals.c -o exec_signals

clean:
	$(RM) infinite_pause basics fork_signals exec_signals
