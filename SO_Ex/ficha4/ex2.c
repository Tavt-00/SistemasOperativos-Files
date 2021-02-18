#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>


/*USANDO O SIGNAL()*/

/*neste jogo se o user demorar x segundos a responder é avisado o TIMEOUT e é dado mais 10s, desde que ainda tenha errado menos que 3
se responder mesmo que seja errado é tirado 1s
se acertar fica com o mesmo tempo
se errar 3 o jogo acaba*/

int pontos = 0, erradas = 0, tempo = 15;


int fim(){
	
	printf("\nPontuacao final: %d\n", pontos);
	printf("Errou %d, dumb\n", erradas);
	printf("XAU\n");

	exit(0);
}


void timeout(int s){	//avisar que esgotou o tempo, incrementamos as erradas e damos mais tempo para jogar

	printf("\nTIMEOUT!!\n");
	fflush(stdout);
	
	erradas++;
	tempo--;

	if(erradas==3){
		printf("Atingiu o max de erradas! Jogo acabou...\n");
		fim(); //mostrar pontuacao e sair
	}
	else{
		printf("Ainda pode responder em %ds\n", tempo);
		alarm(tempo); //dar mais 10s para jogar, se esses 10 forem usados, entra de novo nesta funcao
	}

}


int main(int argc, char const *argv[]){
	
	int num2, num1, resposta;
	
	signal(SIGALRM, timeout);

	do{

		srand(time(NULL));
	    num1 = rand() % 11;
	    num2 = rand() % 11;

	    printf("\nTempo disponivel: %d\n", tempo);
	    printf("Soma de %d + %d = ", num1, num2);
	    fflush(stdout);

	    alarm(tempo);	//espera por uma resposta

	    scanf("%d", &resposta);
	    alarm(0);	//se veio para aqui é porque nao esgotou o tempo


	    if (resposta == num1 + num2){
	    	pontos++;
	    	printf("ACERTOU!\n");
	    }
	    else{
	    	erradas++;
	    	tempo--;
	    	printf("ERROU!\n");
	    }

	    if (erradas == 3){		//quando atingir duas erradas acaba
	    	printf("Atingiu o max de erradas! Jogo acabou...\n");
	    	break; //sai deste do logo
	    }


	}while(tempo > 0);

	fim(); //mostrar pontuacao

	return 0;
}

