#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <sys/wait.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <string.h>

#define FIFO_SRV "tubo"   //pipe do cliente para o servidor ler
#define FIFO_CLI "cli"	  //pipe para cada cliente, serv escreve resposta em cada pipe de cada cliente

/*cli quer obter a pergunta do user e enviar para o cliente
de seguida quer ler a resposta*/


typedef struct{
	int num1, num2;
	char op; // operadores-> +, -, *, /, s (sair) 
	float res;
}PEDIDO;


int main(int argc, char const *argv[]){
	

	int fd, n, fdR;
	PEDIDO p;

		//obter pedido do user
	printf("Num1: ");
	scanf("%d", &p.num1);

	printf("Operacao desejada: ");
	scanf(" %c", &p.op);

	printf("Num2: ");
	scanf("%d", &p.num2);

		//abrir fifo do serv para lhe enviar a pergunta
	fd = open(FIFO_SRV, O_WRONLY); 

	n = write(fd, &p, sizeof(PEDIDO));

	printf("Pergunta enviada pelo fifo do SRV ao SRV\n");

	
		// criar fifo do cliente
	if(access(FIFO_CLI,F_OK)!= 0){	//se ainda nao exisita
		mkfifo(FIFO_CLI,0600);
	}

	printf("Fifo do cliente criado\n");

		//obter a respota à pergunta que o SRV envio para o meu fifo 
		//(analogia de que o prof responde aos alunos para o email de cada um)
	fdR = open(FIFO_CLI, O_RDONLY); 

		//nao precisamos de fazer ciclo como no srv porque aqui apenas se abre uma vez para ler a resposta
		//ja o srv tem de ler e responder a varios clientes
	n = read(fdR, &p, sizeof(PEDIDO));
	close(fdR); 

		//ja se abriu o fifo onde me escreveram a resposta, posso fechar e apresentar ao user
	printf("\nResultado da conta: %f\n", p.res);

	unlink(FIFO_CLI); 

	exit(1);
}
