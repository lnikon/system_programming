#include <stdio.h>
#include <pthread.h>
#include <assert.h>
#include <string.h>
#include <stdlib.h>

// TODO: Synchronization & mutex & conditional variables
// Kernel thread, user thread, green tread, light thread, coroutines

struct Coords
{
	int x;
	int y;
};

void* mult_coords(void* pCoordsVoid)
{
	assert(pCoordsVoid);

	Coords* pCoords = (Coords*)pCoordsVoid;
	pCoords->x = 2 * pCoords->x;
	pCoords->y = 2 * pCoords->y;

	int* result = (int*)malloc(sizeof(int));
	*result = pCoords->x * pCoords->y;
	return (void *)result;
}

int main()
{
	pthread_t p;

	Coords c;
	c.x = 2;
	c.y = 2;

	pthread_create(&p, NULL, mult_coords, (void*)&c);

	int* result;
	pthread_join(p, (void**)&result);

	printf("result=%d\n", *result);
	free(result);

	return 0;
}

