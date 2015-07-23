#ifndef THREADCALLBACK_H
#define THREADCALLBACK_H
#include "Acceptor.h"

void* accept_thread(void *args);
void* connect_thread(void *args);
void* send_thread(void *args);
void* recv_thread(void *args);



#endif // THREADCALLBACK_H
