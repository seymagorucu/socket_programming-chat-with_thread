#include<stdio.h>
#include<stdlib.h>
#include<sys/types.h>
#include<sys/socket.h>
#include<string.h>
#include<netinet/in.h>
#include<pthread.h>
#include<errno.h>
#include<netdb.h>

#define MAX 1024


void * recvMessage(void * socket);

