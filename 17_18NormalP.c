1.
b. Apresente no ecra o numero de pastas exisitentes em /etc cujo nome comece por "li" e que tenham permissões de escrita em pelo menos um dos niveis.

->ls -l /etc | tr -s " " | cut -d " " -f1,9 | grep "^d" | grep "li" | cut -d " " -f1 | grep "w" | wc -l

	FAZER POR PARTES CADA COISA, primeiro fui buscar a parte das permissões e do nome apenas. E dessas duas coisas, vi todos os que começam por "d" e desses os que têm um "li".
	Depois como queria saber apenas onde havia "w" na parte das permissões, cortei apenas para essa parte e pesquisei pelo "w"


2.
a. 

i.
typedef struct 
{
	int id;
	int lotacao;
	int tempo;		//tempo disponivel para comprar bilhetes
	float precoBi;
	char nome[10];
	pthread_mutex_t *ptrinco;
}ESPETACULO;

typedef struct 
{
	char nomeFifo[40];
	char comando[40];
	int idEspetaculo;
	int quantidade;
	int comprou;	//flag para o cliente saber se comprou o nao os bilhetes que queria
	ESPETACULO e;	//ligacao ao espetaculo 
}CLIENTE;


Iria existir um named pipe do servidor (criado por ele) e seria usado para os clientes escreverem os seus pedidos nele. Já o servidor usava-o para ler esses pedidos.
Também ia existir um named pipe para cada cliente, criado também por cada cliente, usado para o servidor escrever as respostas aos pedidos dos clientes e consequentemente, os clientes iriam usá-lo para ler as mesmas.

ii.
No servidor iria ser usado o mecanismo do select para o mesmo conseguir ler pedidos diretamente do teclado e ainda conseguir ler/escrever nos pipes.
Também usaria threads para a compra dos bilhetes e para contar o tempo de compra.

No cliente, caso o mesmo obtesse a quantidade de bilhetes a comprar e o ID do espetaculo através do utilizador, também seria necessário o uso do mecanismo do select.


b.

//servidor
int main(int argc, char const *argv[]){

	fd_set fds; 
	char cmd[40];
	int fdLe, n;
	CLIENTE cli;


	if(access(FIFO_SRV,F_OK) != 0)
		mkfifo(FIFO_SRV,0600);
	else
		exit(2);

	fdLe = open(FIFO_SRV, O_RDWR);

	do{
		FD_ZERO(&fds);	//limpar o conjunto de descritores de ficheiros 
		FD_SET(0,&fds); // atento ao STDIN (info vem da entrada de dados, scanf() de comandos)
		FD_SET(fd, &fds);

		res = select(fdLe+1,&fds,NULL,NULL, NULL);

		if(res == 0){
			printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
		}
		else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo stdin
			
			printf("Comando: ");fflush(stdout);
			scanf("%s", cmd); 	

			if(strcmp(cmd, "adicionar") == 0){
				//tratar de adicionar um espetáculo
			}
			else if(strcmp(cmd, "terminar") == 0){
				//tratar de sair como deve ser
				break;	
			}
		}

		else if(res > 0 && FD_ISSET(fd,&fds)){	//se vier pelo fifo do arbitro
			n = read(fdLe, &cli, sizeof(CLIENTE));

			if(n == sizeof(CLIENTE)){
				if (strcmp(cli.comando, "comprar") == 0){
					//tratar de comprar bilhetes ao cliente
				}
			}
		}


	}while(1);

	close(fdLe);
	exit(0);
}


c + d.

void *conta(void *dados){

	TDATA *pdata;
    pdata = (TDATA *)dados; 

    pthread_mutex_lock(pdata->ptrinco);
    sleep(pdata->tempo);
    pthread_mutex_unlock(pdata->ptrinco);

    int *res;

    res = (int *) malloc(sizeof(int)); 
    *res = 0;
    pthread_exit(res); 
}

void *compra (void *dados){

	TDATA *pdata;
    pdata = (TDATA *)dados; 

    pthread_mutex_lock(pdata->ptrinco);
    pdata->e.lotacao - pdata->quantidade;
    pthread_mutex_unlock(pdata->ptrinco);


    int *res;

    res = (int *) malloc(sizeof(int)); 
    *res = 1;
    pthread_exit(res); 
}


int main(int argc, char const *argv[]){

	fd_set fds; 
	char cmd[40];
	int fdLe, n;
	CLIENTE cli;
	ESPETACULO esp[20];
	int contaEsp = 0;
	int *resultado, *resultado2;
	int contaCli = 0;

	pthread_t tempo, compra;
	pthread_mutex_t trinco;

	pthread_mutex_init(&trinco,NULL);

	if(access(FIFO_SRV,F_OK) != 0)
		mkfifo(FIFO_SRV,0600);
	else
		exit(2);

	fdLe = open(FIFO_SRV, O_RDWR);

	do{
		FD_ZERO(&fds);	//limpar o conjunto de descritores de ficheiros 
		FD_SET(0,&fds); // atento ao STDIN (info vem da entrada de dados, scanf() de comandos)
		FD_SET(fd, &fds);

		res = select(fdLe+1,&fds,NULL,NULL, NULL);

		if(res == 0){
			printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
		}
		else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo stdin
			
			printf("Comando: ");fflush(stdout);
			scanf("%s", cmd); 	

			if(strcmp(cmd, "adicionar") == 0){
				if(contaEsp == 20){
					printf("Nao pode adicionar mais esp.\n");
					break;
				}
				
				printf("NOME: ");
				scanf("%s",esp[contaEsp].nome );
				
				printf("ID: ");
				scanf("%d",esp[contaEsp].id );
					
				printf("TEMPO: ");
				scanf("%d",esp[contaEsp].tempo);
					
				printf("PRECO: ");
				scanf("%d",esp[contaEsp].precoBi);
					
				printf("LOTACAO: ");
				scanf("%d",esp[contaEsp].lotacao);

				esp[contaEsp].ptrinco = &trinco;
				contaEsp++;
				
				pthread_create(&tempo, NULL, conta, (void *)&esp[contaEsp]);
				pthread_join(tempo,(void *)&resultado); 
				*resultado = esp[contaEsp]tempo;	//meter a 0 o tempo

				free(resultado);
			}
			else if(strcmp(cmd, "terminar") == 0){
				//tratar de sair como deve ser
				break;	
			}
		}

		else if(res > 0 && FD_ISSET(fd,&fds)){	//se vier pelo fifo do arbitro
			n = read(fdLe, &cli, sizeof(CLIENTE));

			if(n == sizeof(CLIENTE)){
				if (strcmp(cli.comando, "comprar") == 0){
						
						for (int i = 0; i <20 ; i++){
							if(cli.idEspetaculo == esp[i].id){
								cli.e = esp[i];

								if(esp[i].lotacao - cli.quantidade >= 0){
									cli.e.ptrinco = &trinco;
									
									pthread_create(&compra,NULL, compra, (void *)&cli);
									
									pthread_join(compra,(void *)&resultado2); 
									*resultado2 = cli.comprou;	//comprou os bilhetes

									int fdEsc = open(cli.nomeFifo, O_WRONLY);
									int m = write(fdEsc, &cli, sizeof(CLIENTE));
									close(fdEsc);

									free(resultado2);
									break;
								}
									
							}
						}
					}
				}
			}

	}while(1);
	
	close(fdLe);
	exit(0);
}


//CLIENTE

//servidor envia um SIGUSR1 quando termina

#define FIFO_CLI "cli%d"

int flagSai = 0;

void sai(int s){
	flagSai = 1;
}


int main(int argc, char const *argv[]){
	
	int id = getpid();

	struct sigaction act;

	act.sa_sigaction = sai;
	act.sa_flags = SA_SIGINFO;	

	sigaction(SIGUSR1, &act, NULL);

	if(flagSai == 1){
		unlink(FIFO_CLI);
		exit(0);
	}

}