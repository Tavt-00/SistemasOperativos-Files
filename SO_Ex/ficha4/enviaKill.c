#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/type.h>

//com o kill apenas podemos enviar o sinal e mais nada

int main(int argc, char const *argv[]){
	int pid, sinal;


	if(argc != 3){
		fprintf(stderr, "[ERRO] Nr. de argumentos!\n       ./envia SIGNAL PID\n");
		exit(1);
	}

	sinal = atoi(argv[1]);
	pid = atoi(argv[2]);

	printf("Vou enviar o sinal %d ao processo com o pid %d ...\n", sinal,pid);

	kill(pid,sinal);	//envia o sinal que está em "sinal" para o processo que tem aquele "pid"


	exit(0);
}