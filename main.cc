#include "mmappool.h"
#include "shmpool.h"
#include <stdio.h>


static void
mmap_pool()
{
	mmap_pool_t<const char*> pool(64);
	
	pool.create("shmpool.data", 1024);
	
	printf("used : %lu \n", pool.used());
	printf("unused : %lu \n", pool.unused());
}

static void
shm_pool()
{
	shm_pool_t<key_t> pool(64);
	
	pool.create(0x12345, 1024);
	
	printf("used : %lu \n", pool.used());
	printf("unused : %lu \n", pool.unused());	
}

int
main(int argc, char* arv[])
{
	shm_pool();
	
	return 0;
}
