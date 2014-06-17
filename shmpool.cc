#include "shmpool.h"
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>
int 
shm_pool_t::create(const char* file, unsigned long len)
{
	int ret;
	
	this->_file = file;
	this->_size = len;
	
	ret = this->__shm_create();
	
	if (ret == -1) return ret;
	
	ret = this->__pool_create();
	
	return ret;
}

int 
shm_pool_t::__shm_create()
{
	int fd = open(_file.c_str(), O_CREAT | O_TRUNC | O_RDWR, 00777);
	
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
	int 	n;
	void*	s; 
	
	n = _size / _value;
	
	for(int i = 0; i < n; i++) {
		s = (char*)_pool + i * _value;
		_free.insert(s);
	}
	
	return _free.size();
}

int
shm_pool_t::__pool_delete()
{
	_use.clear();
	_free.clear();
}

