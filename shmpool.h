#ifndef __SHM_POOL_H_
#define __SHM_POOL_H_

#include "smpool.h"
#include <sys/types.h>
#include <unistd.h>
#include <sys/ipc.h>
#include <sys/shm.h>

template<class U>
class shm_pool_t : public sm_pool_t<U> {
public:
	shm_pool_t(unsigned int value):
		sm_pool_t<U>(value),
		_shmid(0),
		_key(0)
	{}
	
	~shm_pool_t() {__pool_delete(); __shm_delete();}
private:
	int __shm_create(U u);
	int __shm_delete();
	int __pool_create();
	int __pool_delete();
private:
	int 	_shmid;
	key_t	_key;
};

template<class U> int 
shm_pool_t<U>::__shm_create(U u)
{
	_key = u;
	_shmid = shmget(_key, this->_size, IPC_CREAT | 0666);
	
	if ( _shmid == -1 ) return -1;
	
	this->_pool = shmat(_key, NULL, 0);
	
	if (this->_pool == NULL) return -1;
	
	return 0;
}

template<class U> int 
shm_pool_t<U>::__shm_delete()
{
	shmctl(this->_shmid, IPC_RMID, NULL);
	this->_pool = NULL;
	this->_size = 0;
	this->_key = 0;
	return 0;
}

template<class U> int 
shm_pool_t<U>::__pool_create()
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
shm_pool_t<U>::__pool_delete()
{
	this->_use.clear();
	this->_free.clear();
	
	return 0;
}

#endif 

