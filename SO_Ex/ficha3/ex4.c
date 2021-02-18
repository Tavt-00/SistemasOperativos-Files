#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>

int main () {

	char str[40];
	int flag = 1, pid, res, saida;


	pid = getpid();  //getppid() dá o pid do pai

	while(flag == 1){

		printf("[%d] Introduza o comando: ", pid);
		fflush(stdout);
		scanf("%s", str);

		if (strcmp(str, "sair") != 0){

			res = fork();
			if(res == 0){	//ESTAMOS NO FILHO, ele é que faz o exec visto que o criamos para ele ser outro programa
				pid = getpid();
				printf("Sou o FILHO %d\n", pid);
				execl(str,str, NULL);	//executar o que o user vai meter no comando´
				//o primeiro é o nome do executavel, o segundo e por aí em diante são os parametros que a linha de comandos precisa para executar o programa

				printf("[ERRO] Nao consegui executar o programa! --> [%d]\n", pid);	//veio para aqui é pq o exec nao funcionou
				exit(7); //exec nao funcionou, terminar o programa
			}
			else{//ESTAMOS NO PAI, veio primeiro para ao pai, então o fork devolveu o pid do filho que foi criado

				printf("Sou o PAI %d, vieste primeiro ao pai\n", pid);
				printf("O meu filho tem o pid igual a %d\n",res);
				//pai tem de epserar pelo que o filho devolve antes de começar a fazer novamente algo
				//nem que espere por um valor que significa que o filho correu mal 
				

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








/*so com o exec, o programa do pai morre para o processo dele dar vida ao programa do exec

--> PARA RESOLVER ISTO USAMOS O FORK

O FORK cria um processo novo com um programa igual ao pai e nesse processo podemos por a correr o programa que quisermos, sendo que o pai nunca morre.

->Ao fazer o fork, se o mesmo devolver o valor 0, estamos já no filho, se devolver diferente de 0 ainda estamos no pai (devolver o valor do pid do filho que foi criado)
O fork pode correr primeiro o pai ou o filho. executa um deles e depois logo o outro. Ou seja, faz sempre duas vezes, uma para o pai e outra para o filho
