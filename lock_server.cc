// the lock server implementation

#include "lock_server.h"
#include <sstream>
#include <stdio.h>
#include <unistd.h>
#include <arpa/inet.h>



lock_server::lock_server():
  nacquire (0)
{
}

lock_protocol::status lock_server::stat(int clt, lock_protocol::lockid_t lid, int &r)
{
  lock_protocol::status ret = lock_protocol::OK;
  printf("stat request from clt %d\n", clt);
  r = nacquire;
  return ret;
}

lock_protocol::status lock_server::acquire(int clt, lock_protocol::lockid_t lid, int &r) 
{
	lock_protocol::status ret = lock_protocol::OK;
	if(lock_clt_map.find(lid) == lock_clt_map.end())
	{
		new_lock(clt, lid);
	}
	pthread_mutex_lock(&lock_mutex_map[lid]);

	if(active_lock.find(lid) == active_lock.end())
	{
		pthread_cond_wait(&lock_cond_map[lid], &lock_mutex_map[lid]);
	}
	active_lock.erase(lid);
	lock_clt_map[lid] = clt;
	pthread_mutex_unlock(&lock_mutex_map[lid]);

	return ret;
}

lock_protocol::status lock_server::release(int clt, lock_protocol::lockid_t lid, int &r) 
{
	lock_protocol::status ret = lock_protocol::OK;
	pthread_mutex_lock(&lock_mutex_map[lid]);

	if(lock_clt_map[lid] == clt)
	{
		active_lock.insert(lid);
		pthread_mutex_signal(&lock_cond_map[lid]);
	}
	pthread_mutex_unlock(&lock_mutex_map[lid]);
	return ret;
}

void lock_server::new_lock(int clt, lock_protocol::lockid_t lid)
{
	ScopedLock slock(&rwlock);

	pthread_mutex_t mtx;
	pthread_cond_t cond;

	pthread_mutex_init(&mtx, NULL);
	pthread_cond_init(&cond, NULL);

	lock_clt_map[lid] = clt;
	lock_mutex_map[lid] = mtx;
	lock_cond_map[lid] = cond;

	active_lock_insert(lid);
}

