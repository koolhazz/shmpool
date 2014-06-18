#ifndef __MMAP_POOL_H_
#define __MMAP_POOL_H_

#include "smpool.h"
#include <string>
#include <unistd.h>
#include <sys/mman.h>
#include <fcntl.h>

using std::string;

template<class U>
class mmap_pool_t : public sm_pool_t<U> {
public:
	mmap_pool_t(unsigned int value):
		sm_pool_t<U>(value),
		_fd(0)
	{}
	~mmap_pool_t() {__pool_delete(); __shm_delete();}
private:
	int __shm_create(U u);
	int __shm_delete();
	int __pool_create();
	int __pool_delete();
private:
	int 	_fd;
	string 	_file;
};

template<class U> int 
mmap_pool_t<U>::__shm_create(U u)
{
	this->_file = u;
	this->_fd = open(_file.c_str(), O_CREAT | O_TRUNC | O_RDWR, 00777);
	
	if (this->_fd <= 0) return -1;
	
	this->_pool = mmap(NULL, this->_size, PROT_READ | PROT_WRITE, MAP_SHARED, this->_fd, 0);
	
	if ( this->_pool == NULL) return -1;
	
	return 0;
}

template<class U> int 
mmap_pool_t<U>::__shm_delete()
{
	munmap(this->_pool, this->_size);
	close(this->_fd);
	this->_pool = NULL;
	this->_size = 0;
	this->_fd = -1;
	unlink(this->_file.c_str());
	
	return 0;
}

template<class U> int 
mmap_pool_t<U>::__pool_create()
{
	int 	n;
	void*	s; 
	
	n = this->_size / this->_value;
	
	for(int i = 0; i < n; i++) {
		s = (char*)this->_pool + i * this->_value;
		this->_free.insert(s);
	}
	
	return this->_free.size();
}

template<class U> int
mmap_pool_t<U>::__pool_delete()
{
	this->_use.clear();
	this->_free.clear();
	
	return 0;
}

#endif 
