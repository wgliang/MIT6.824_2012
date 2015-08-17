// this is the lock server
// the lock client has a similar interface

#ifndef lock_server_h
#define lock_server_h

#include <string>
#include "lock_protocol.h"
#include "lock_client.h"
#include "rpc.h"

class lock_server {

 protected:
  int nacquire;

 public:
  lock_server();
  ~lock_server() {};
  
  std::map<lock_protocol::lockid_t, pthread_mutex_t> lockid_mutex_map;
  //map<lock_id, pthread_cond>
	std::map<lock_protocol::lockid_t, pthread_cond_t> lockid_cond_map;
	std::map<lock_protocol::lockid_t, int> lockid_clt_map;
  lock_protocol::status stat(int clt, lock_protocol::lockid_t lid, int &);

  lock_protocol::status acquire(int clt, lock_protocol::lockid_t lid, int &);//get lock
  lock_protocol::status release(int clt, lock_protocol::lockid_t lid, int &);//release lock
  void new_lock(int clt, lock_protocol::lockid_t lid);
};

#endif 







