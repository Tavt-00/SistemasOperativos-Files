#include "ex2Utils.h"

int continua = 1;


/*funcao de tratamento de sinal*/
void termina(int s){
    continua = 0;	//desliga a flag qd recebe o sinal
}


// ./enviaThrd esq p.exemplo
int main(int argc, char const *argv[]){

	PEDIDO p;
	int fdSrv, n, scan;

	struct sigaction act;
    act.sa_handler = termina;
    act.sa_flags = 0;

    	/*VERIFICA SE TEM ARGUMENTOS DA LINHA DE COMANDOS*/
	if (argc != 2){
		printf("ERRO, Numero de argumentos\nA sair...\n");
		exit(2);
	}
		

	    /*ABRE FIFO DO ARGV[1] PARA ESCRITA*/
	fdSrv = open(argv[1], O_WRONLY);

	if (fdSrv != -1){
		
			/*PEDE NOME*/
		printf("Nome? \n");
		scanf("%s", p.nome);

			/*GUARDA PID*/
		p.pid = getpid();

		  	/*TRATA DO SINAL*/
		sigaction(SIGUSR1, &act, NULL);

		do{
        		/*PEDE NUMERO*/
        	printf("Numero? \n");
        	scan = scanf("%d", &p.num);
	    	
	    	if (scan == 1){	//é porque recebeu um inteiro
	   			
	   			if(p.num == 0)
        			break;

        			/*ESCREVE NO FIFO*/
    			n = write(fdSrv, &p, sizeof(PEDIDO));
    			printf("Enviei para o fifo %s: '%s', '%d' \n", argv[1], p.nome, p.num);
	    	}
        	else
        		break;
	
		}while(continua);
		
	}
	else{
		printf("ERRO, named pipe não existe...\n");
		exit(2);
	}
		

    	/*FECHA O FIFO*/
	close(fdSrv);

	printf("A sair...\n");
	exit(0);
}
