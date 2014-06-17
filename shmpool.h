#ifndef __SHM_POOL_H_
#define __SHM_POOL_H_

#include <string>
#include <tr1/unordered_set>

using std::tr1::unordered_set;
using std::string

class shm_pool_t {
public:
	shm_pool_t() {}
	~shm_pool_t() {}
public:
	int create(const char* file, unsigned long len);
	int shink();
	void* alloc(unsigned long size);
	void free(void* ptr);
private:
	int __shm_create();
	int __shm_delete();
	int __pool_create();
	int __pool_delete();
private:
	string 					_file;
	unsigned long 			_size;
	unsigned int			_wmk;
	int						_fd;
	void*					_pool;
	unordered_set<void*> 	_use;
	unordered_set<void*> 	_free;
};

#endif 