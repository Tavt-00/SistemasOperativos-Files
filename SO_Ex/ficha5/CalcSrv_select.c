#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>
#include <sys/select.h>
#include <sys/time.h>



#define FIFO_SRV "tubo"
#define FIFO_CLI "cli"

typedef struct{
	int num1,num2;
	char op; // +,-,*, / e s-> sair
	int res;
	int pid;
}PEDIDO;


int main(int argc, char const *argv[]){

	setbuf(stdout,NULL);
	int fd,n_bytes, fdResposta,res;
	PEDIDO p;
	char fifo[40],cmd[40];
	fd_set fds;
	struct timeval tempo;


	//verificar se existe
	if(access(FIFO_SRV,F_OK)!= 0){// != 0 , entao ficheiro nao existe

		// criar fifo do servidor
		mkfifo(FIFO_SRV,0600);
	}

	fd = open(FIFO_SRV,O_RDWR); // Abri para leitura

	printf("Abri o '%s' ...\n",FIFO_SRV);


	do{
		
		//utilizar select para saber de onde vem informaçao para avisarmos o srv
		
		FD_ZERO(&fds);	//limpar o conjunto de descritores de ficheiros 
		FD_SET(0,&fds); // atento ao STDIN (info vem da entrada de dados, scanf() de comandos)
		FD_SET(fd, &fds); // atento ao FIFO (info vem de um fifo, read() de pedidos)
		
		tempo.tv_sec = 10; // TIMEOUT, tempo de espera por dadoss
		tempo.tv_usec = 0;


		res = select(fd+1,&fds,NULL,NULL, &tempo); //BLOQUEIA AQUI
			//o select bloqueia o srv e avisa-o quando algo chegar e diz-lhe o que é (um jogo, cliente, etc) 

		if(res == 0){
			printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
		}
		else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo stdin

			// ler comandos do administrador, informaç~ao que veio pela entrada, stdin
			printf("COMANDO: ");fflush(stdout);
			scanf("%s", cmd); 	// bloqueia enquanto não há dados - mas como temos o select bloqueia nele e so vem para aqui havendo dados

			printf("O administrador pediu o comando '%s' ...\n",cmd);
			if(strcmp(cmd,"termina") == 0)
				p.op = 's';
		}

		else if(res > 0 && FD_ISSET(fd,&fds)){ //se vier pelo fifo

		// ler info que veio pelo pedido do fifo
		n_bytes = read(fd, &p, sizeof(PEDIDO));   // bloqueia enquanto não há dados - mas como temos o select bloqueia nele e so vem para aqui havendo dados
		printf("Recebi ... %d, %c, %d (%d bytes)\n", p.num1,p.op, p.num2, n_bytes);

			if(n_bytes == sizeof(PEDIDO)){

				p.res = p.num1 + p.num2;

				// envia resposta para o cliente 
				sprintf(fifo,FIFO_CLI,p.pid);

				fdResposta = open(fifo,O_WRONLY); // Abrir para escrita o fifo do cliente
				n_bytes = write(fdResposta, &p, sizeof(PEDIDO)); // Escrever nesse fifo
				
				close(fdResposta);
				printf("Escrevi ... %d (%d bytes)\n", p.res, n_bytes);
			}

		}

		

	}while(p.op != 's');

	// Fechei
	close(fd); 

	// apagar fifo do servidor
	unlink(FIFO_SRV);

	exit(0);
}

/*no tp tinhamos:
 o pipe anonimo com o jogo
 o teclado de onde vêm os comandos proprios do srv
 e os fifos de cada cliente */