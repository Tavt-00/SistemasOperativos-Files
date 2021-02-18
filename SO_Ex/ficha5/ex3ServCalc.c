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


/*serv quer ler a pergunta do cliente, calcular a pergunta e escrever a resposta*/


/*analogia ao email, prof tem um email para onde todos os alunos enviam as suas mensagens para ele ler
prof repsonde depois a cada aluno usando o email de cada e não um para todos*/


/*===========================================
aqui, abrimos uma vez o fifo de leitura e fica sempre aberto enquanto houver alguem a mandar algo para ler
	->nao faz sentido estar sempre a abrir e fechar sempre que alguem quer mandar algo
	->fecha-se quando souber que ja nao há nada para ler

para responder, abrimo o tubo do cliente para o qual queremos responder e fechamos, pois para outro cliente já usamos outro tubo
	-> clientes diferentes têm tudos diferentes
=============================================*/



typedef struct{
	int num1, num2;
	char op; // operadores-> +, -, *, /, s (sair) 
	float res;
}PEDIDO;


int main(int argc, char const *argv[]){

	int fd, n, fdR;
	PEDIDO p;

//	access(FIFO_SRV, F_OK); // se devolver 0 -> ficheiro existe

	/*criar fifo do servidor*/
	if(access(FIFO_SRV, F_OK) != 0){ 
		mkfifo(FIFO_SRV,0600);
	}


	//criar o fifo para onde todos os clis mandam pedidos ao serv
	printf("Criei o FIFO para o SRV\n");

	// abri o fifo para ler e deixamos sempre aberto 
	//para nao andar a abrir e fechar sempre que é preciso escrever
	fd = open(FIFO_SRV,O_RDONLY);

	do{
		/*ler pedido do fifo que tras a pergunta do cliente*/
		n = read(fd, &p, sizeof(PEDIDO));
		printf("Recebi... %d %c %d (%d bytes)\n", p.num1, p.op, p.num2,n);

		if(n == sizeof(PEDIDO)){
			/*calcula a pergunta*/
			if(p.op == '+'){
				p.res = p.num1 + p.num2;
			}
			else if(p.op == '-'){
				p.res = p.num1 - p.num2;
			}
			else if(p.op == '*'){
				p.res = p.num1 * p.num2;
			}
			else if(p.op == '/'){
				p.res == p.num1 / p.num2;
			}
			else if(p.op == 's'){
				printf("A sair...");
				break;
			}
			/*envia resposta para o cliente ...*/
			fdR = open(FIFO_CLI, O_WRONLY);
			n = write(fdR, &p, sizeof(PEDIDO));
			close(fdR);

			printf("Escrevi... %d %c %d (%d bytes)\n", p.num1, p.op, p.num2,n);
			
		}
		

	}while(p.op!='s');

	// fecho o fifo de leitura agora
	close(fd); 


	/*apagar fifo do servidor*/
	unlink(FIFO_SRV);



	exit(0);
}