#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>


typedef struct {		//cliente
    char nome[20];
    int num;
    int pid;

}PEDIDO;


typedef struct {		//threads
    char nomeFifo[20];
    int continua;
    pthread_mutex_t *ptrinco;

}TDATA;