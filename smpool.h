#ifndef __SM_POOL_H_
#define __SM_POOL_H_

#include <tr1/unordered_set>
#include <string>

using std::string;
using std::tr1::unordered_set;

class nocopyable_t {
public:
	nocopyable_t() {}
	virtual ~nocopyable_t() {}
private:
	nocopyable_t(const nocopyable_t& o) {}
	nocopyable_t(const nocopyable_t* o) {}
};

template<class T>
class sm_pool_t : public nocopyable_t{
public:
	sm_pool_t(unsigned value):
		_size(0),
		_wmk(75),
		_pool(NULL),
		_value(value)
	{}
	virtual ~sm_pool_t() {}
public:
	int create(T t, unsigned long len);
	int shink();
	void* alloc(unsigned long size);
	void free(void* ptr);
	unsigned long unused() const { return _free.size() * _value; }
	unsigned long used() const { return _use.size() * _value; }
private:
	virtual int __shm_create(T t) = 0;
	virtual int __shm_delete() = 0;
	virtual int __pool_create() = 0;
	virtual int __pool_delete() = 0;
protected:
	unsigned long 			_size;
	unsigned int			_wmk;
	void*					_pool;
	unsigned int			_value;
	unordered_set<void*> 	_use;
	unordered_set<void*> 	_free;
};

template<class T> int
sm_pool_t<T>::create(T t, unsigned long len)
{
	int ret;
	this->_size = len;
	
	ret = this->__shm_create(t);
	
	if (ret == -1) return ret;
	
	ret = this->__pool_create();
	
	return ret;
}


#endif
