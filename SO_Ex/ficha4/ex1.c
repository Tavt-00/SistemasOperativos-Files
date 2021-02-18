#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>

//não podemos alterar os parametros da funcao que trata do sinal
// s é o numero do sinal que levou a chamar esta funcao

void mostra (int s){
	printf("\nACORDA!!! (sinal que foi chamado: %d)",s);
	fflush(stdout);
	alarm(5);	//se estees 5s passarem, o alarm volta a mandar um SIGALRM, que
				// volta para aqui e volta a fazer isto, sempre enquanto o user n colocar o nome, qd colocar temos o alar(0)
	//exit(33);
}


int main(int argc, char *argv[]){
	int i;
	char str[40];

	//signal diz o que fazer com um sinal, neste caso dizemos que é para ignorar
//	signal(SIGINT, SIG_IGN);//usar o signal para dizer o que fazer com o sinal que aparece no primeiro campo e dizemos o que fazer no segundo campo

	//neste caso dizemos que faz o que está definido em default
//	signal(SIGINT, SIG_DFL);

	//neste caso dizemos que vai fazer o que esta na funcao mostra
//	signal(SIGINT, mostra);
//  signal(SIGUSR1, mostra);
	signal(SIGALRM, mostra);

	printf("O meu PID e %d...\n",getpid());

	/*printf("\nVou dormir.....\n");	//nao faço nada a partir daqui
	alarm(10);
	do{
		pause();
	}while(1);*/


	do{
		alarm(10);	//contar 10 segundos com o alarm que manda um SIGALARM no fim, tratamos dele em cima
								//o user tem 10s para responder ao nome, se não é acionado o alarm
		printf("Nome? ");
		fflush(stdout);
		scanf("%s",str);

		alarm(0);	//se veio para aqui é porque o user ja colocou o nome, podemos desligar o alarm, já não precisamos que ele toque e manda um SIGALARM

		printf("Ola");
		for(i = 0; i < 5;i++){
			printf("a");
			fflush(stdout);
			sleep(1);
		} 
		printf(" %s!!!\n",str);

	}while(strcmp(str,"sair") != 0);


	exit(0);
}