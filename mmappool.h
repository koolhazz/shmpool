#ifndef __SHM_POOL_H_
#define __SHM_POOL_H_

#include <string>
#include <tr1/unordered_set>

using std::tr1::unordered_set;
using std::string;

class mmap_pool_t {
public:
	mmap_pool_t(unsigned int value):
		_size(0),
		_wmk(75),
		_fd(-1),
		_pool(NULL),
		_value(value)		
	{}
	~mmap_pool_t() {__pool_delete();}
public:
	int create(const char* file, unsigned long len);
	int shink();
	void* alloc(unsigned long size);
	void free(void* ptr);
	unsigned long unused() const { return _free.size() * _value; }
	unsigned long used() const { return _use.size() * _value; }
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
	unsigned int			_value;
	unordered_set<void*> 	_use;
	unordered_set<void*> 	_free;
};

#endif 
