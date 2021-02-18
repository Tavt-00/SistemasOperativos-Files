#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <time.h>

/*USADO O SIGACTION()*/

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

	//com o flags = 0, já nao precisamos de repetir parte daquele processo de ver quantas erradas e tal
	//como isto salta para depois do scanf e lá temos tudo isso, não precisamos de fazer nada 
}

void timeout_novo(int s, siginfo_t *info, void *context){
	//conseguimos saber quem enviou o sinal com o si_pid que está na info e o valor
	
	printf("TIMEOUT!!! \n");
	printf("(Sinal = %d, PID = %d, Valor = %d)\n", s, info->si_pid, info->si_value);
	
	if(info->si_pid == 0) // se o sinal for enviado por mim (valor 0) é que contamos
		erradas++;

	//nesta funcao temos o *info 
	//que é uma estrutura onde podemos ter informacoes adicionais que podem dar jeito
}


int main(int argc, char const *argv[]){
	
	int num2, num1, resposta;
	struct sigaction act;	//estrutura para o sigaction
	

#if 0	//SEM A ESTRUTURA na funcao timeout 
	
	act.sa_handler = timeout;
	//act.sa_flags = SA_RESTART;	//com este restart estamos a dizer ao sistema para voltar a tentar ler no scanf e nao sair daí
	act.sa_flags = 0; /*sempre que  passarem os 10 segundos, nao fica parado no scanf
						ou seja, salta fora o scanf e volta ao inicio do codigo
						isto é feito com a sa_flags a 0 porque por defeito o alarm
						usa a flag SA_RESTART*/

	sigaction(SIGALRM, &act, NULL);
			//1º paramettro é o nome do sinal que é para tratar
			//2º parametro é a estrutura do sigaction
			//3º é o comportamento antigo


#else	//COM A ESTRUTURA na funcao timeout, tem de se usar a sa_sigaction
	act.sa_sigaction = timeout_novo;
	act.sa_flags = SA_SIGINFO;	//ao usar siginfo temos de usar em cima sigaction
	sigaction(SIGALRM, &act, NULL);
#endif


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
	    			//com sa_flags = 0 também vem para aqui

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

