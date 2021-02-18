#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>


int main(int argc, char const *argv[])
{
	int i, flag = 0;
	char palavra[10];
	char *tradutor[5][2] = { {"fixe", "belle"}, {"ola", "bonjour"}, {"sim", "oui"}, {"dois", "deux"}, {"quando", "quand"}};

	if(argv[1] == NULL){
		printf("escreva uma palavra em pt para traduzir!\n");
		exit(0);
	}
	else
		strcpy(palavra,argv[1]); //copiar a string recebida de argv[1]


	for(i = 0; i < 5; i++){

		if(strcmp(palavra,tradutor[i][0]) == 0){ //se existir na matriz, mostra a sua tradução
			flag = 1;
			strcpy(palavra,tradutor[i][1]); 
		}
	}

	if (flag){
		printf("Traducao em FRANCES: %s\n",palavra);
		exit(1);
	}
	else{
		printf("unkown\n"); // caso contrário mostra unknown
		exit(0);
	}
	

}