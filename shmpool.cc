#include "shmpool.h"
#include <unistd.h>

int 
shm_pool_t::create(const char* file, unsigned long len)
{
	this->_file = file;
	this->_size = len;
	
	return this->__shm_create();
}

int 
shm_pool_t::__shm_create()
{
	int fd = open(file.c_str(), O_CREAT | O_TRUNC | O_RDWR, 00777);
	
	if (fd <= 0) return -1;
	
	_pool = mmap(NULL, _size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
	
	if ( _pool == NULL) return -1;
	
	return 0;
}

int 
shm_pool_t::__shm_delete()
{
	munmap(_pool, _size);
	close(_fd);
	_pool = NULL;
	_size = 0;
	_fd = -1;
}

int 
shm_pool_t::__pool_create()
{
	return 0;
}

int
shm_pool_t::__pool_delete()
{
	_user.clear();
	_free.clear();
}