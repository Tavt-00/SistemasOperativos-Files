#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char const *argv[]){
	
	int res, estado;
	char letra;
	char palavra[10];


	
	while(letra != 'x'){
		printf("Letra: "); //pede uma letra
		fflush(stdout);
		scanf("%c", &letra);
		
		if (letra == 'i'){

			printf("Palavra a traduzir para Ingles: ");
			fflush(stdout);
			scanf("%s", palavra);

			res = fork();	//criar filho igual a este programa para depois ele ser o programa ingles
			if (res == 0){	//estamos no filho
				execl("ding", "ding", palavra, NULL);	//metemos a palavra tb pois o programa ding precisa dela para correr
				
				printf("ERRO a executar o programa DING\n");	//se veio para aqui depois do exec é pq houve erro
				exit(0);
			}
			//se veio para aqui, entao o pai tem de esperar pelo filho
			wait(&estado);

			if(WIFEXITED(estado)){	//se o filho termina com um exit
				printf("\nO programa DING saiu com %d\n", WEXITSTATUS(estado));	//mostrar com o que é que ele saiu
			}		

		}
		else if(letra == 'f'){
			
			printf("Palavra a traduzir para Frances: ");
			fflush(stdout);
			scanf("%s", palavra);

			res = fork();	//criar filho igual a este programa para depois ele ser o programa frances
			if (res == 0){	//estamos no filho
				execl("dfran", "dfran", palavra, NULL);	
				
				printf("ERRO a executar o programa DFRAN\n");	
				exit(0);
			}
			wait(&estado);

			if(WIFEXITED(estado))
				printf("\nO programa DFRAN saiu com %d\n", WEXITSTATUS(estado));	//mostrar com o que é que ele saiu
		}
	}
		

	printf("\nA sair...\n");

	return 0;
}