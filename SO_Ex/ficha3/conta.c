#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

int main(int argc, char *argv[]){

	int i;

	printf("INICIO...\n");

	for(i = 0; i < 20; i++){ // faço um ciclo com o nr de vezes que o argv[2] vai repetir
		printf("~"); // mostro o argv[2]
		fflush(stdout); // faço flush
		sleep(1); // e faço sleep consoante o valor da variavel t
	}

	printf("\nFIM...\n"); 
	

	exit(3);

}

fechamos o stdout (1) porque nao queremos que as mensagens vao para o ecra.
como essa parte fica livre, abrimos depois o o ficheiro, ao abrir vai para o que estava livre que era o 1
estamos então a meter a saida a ir para o ficheiro pois este ficou no 1