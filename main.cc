#include "shmpool.h"
#include <stdio.h>

int
main(int argc, char* arv[])
{
	shm_pool_t pool(64);
	
	pool.create("shmpool.data", 1024);
	
	printf("used : %lu \n", pool.used());
	printf("unused : %lu \n", pool.unused());
	
	return 0;
}
