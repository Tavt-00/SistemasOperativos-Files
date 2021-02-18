#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <sys/type.h>

//com o sigqueue podemos enviar um sinal e um valor

int main(int argc, char const *argv[]){

	int pid,sinal;

	if(argc != 3){
		fprintf(stderr, "[ERRO] Nr. de argumentos !\n          ./envia SINAL PID\n");
		exit(1);
	}

	sinal = atoi(argv[1]);
	pid = atoi(argv[2]);

	printf("Vou enviar o sinal %d ao processo %d...\n", sinal,pid);
	
	//kill(pid,sinal); // envia um sinal ao processo
	union sigval value;	

	value.sival_int = 666;	//valor que podemos enviar também
	
	sigqueue(pid,sinal,value); // envia sinal, 666 neste caso, ao processo

	exit(0);
}