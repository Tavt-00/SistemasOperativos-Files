#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () {

	char str[40];
	int flag = 1, pid, res, saida;
	int canal[2]; //ficheiro com duas coisas: uma para ler outra para escrever


	pid = getpid();  //getppid() dá o pid do pai

	while(flag == 1){

		printf("[%d] Introduza o comando: ", pid);
		fflush(stdout);
		scanf("%s", str);

		if (strcmp(str, "sair") != 0){

			pipe(canal);	//criar pipe antes do fork para ser herdado pelo filho

			res = fork();
			if(res == 0){	//ESTAMOS NO FILHO, ele é que faz o exec visto que o criamos para ele ser outro programa
				pid = getpid();
				printf("Sou o FILHO %d\n", pid);
				
				close(canal[0]); // fecho o canal de leitura do filho pq ele so quer escrever
				close(1);        // fecho o stdin pq filho nao quer escrever no monitor, ficando esta livre
				dup(canal[1]);   // duplico o canal[1] e vai para a antiga posicao do stdin (aquela que fechei em cima - close(1))
				close(canal[1]); // fecho o original porque como o dupliquei e está na posicao nova, nao preciso de outro

				//so depois é que fazemos o execl
				execl(str,str, NULL);	

				printf("[ERRO] Nao consegui executar o programa! --> [%d]\n", pid);	//veio para aqui é pq o exec nao funcionou
				exit(7); 
			}
			else{//ESTAMOS NO PAI, veio primeiro para ao pai, então o fork devolveu o pid do filho que foi criado

				printf("Sou o PAI %d, vieste primeiro ao pai\n", pid);
				printf("O meu filho tem o pid igual a %d\n",res);
				//pai tem de epserar pelo que o filho devolve antes de começar a fazer novamente algo
				
				close(canal[1]); //pai nao precisa de escrever p/filho, so quer ler

				//começamos a ler o que vem do filho até já nao houver mais para ler
				while((n = read(canal[0],str,39)) != 0){ // == 0 ---> EFO
					//lemos do canal0 para dentro do buffer que é a str
					//apenas 39 para depois no fim meter \0
					
					str[n] = '\0';
					printf("[%5d] PAI : Recebi ... '%s'\n",pid,str);//mostrar o que o pai leu do filho
				}
				
				close(canal[0]); //ja acabamos de ler, podemos fechar a leitura

				wait(&saida); //esperar pelo valor que o filho devolve quando acaba, euqnato o prog do filho nao acabar, nao vemos nada do pai

				if(WIFEXITED(estado)){	//se o filho acabou por causa de um exit
					printf("Sou o PAI, o meu filho acabou com %d\n", WEXITSTATUS(estado));	//se sim, então ir buscar o valor do exit com o WEXITSTATUS
				}
			}
			
		}
		else
			flag = 0; 
	}
	
	exit(0); 
} 

