CC = gcc
CFLAGS = -Wall -pedantic -g -O0
RM = rm

all: basic_shared_memory basic_semaphores sync_producer_consumer

basic_shared_memory: basic_producer.c basic_consumer.c
		$(CC) $(CFLAGS) basic_producer.c -o basic_producer
		$(CC) $(CFLAGS) basic_consumer.c -o basic_consumer

basic_semaphores:
		$(CC) $(CFLAGS) basic_semaphores.c -o basic_semaphores

sync_producer_consumer: sync_producer.c sync_consumer.c
		$(CC) $(CFLAGS) sync_producer.c -o sync_producer
		$(CC) $(CFLAGS) sync_consumer.c -o sync_consumer

clean:
	$(RM) basic_producer basic_consumer basic_semaphores sync_producer sync_consumer
