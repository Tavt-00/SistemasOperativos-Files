1.a.
cat /etc/passwd | tr -s ":" " " | cut -d ":" -f5 | grep "Alves" | wc -l

b.
ls -l | tail -n +2 | grep "^-" |tr -s " " | cut -d " " -f9 | sort -n -r | head -n +3 


2.

#define FIFO_SRV "fifo_srv"
#define FIFO_CLI "fifo_cli%d"

//a)  CAIXA CENTRAL


typedef struct cli cliente, *pcli;
struct cli{
	
	char nomePipe[10];
	int pid;
	TDATA *data;
	pcli prox;
};

typedef struct {
    char *produtos[5][1]; 
    int codigos[5];
    float precos[5]
    char produto_aRegistar[10];
    char pedido[10];
    int pidCli;
}TDATA;

typedef struct{
	int tempo;
}DADOS;


int main(int argc, char const *argv[]){

	char *ncaixas;
	int nCaixas, fdLe;

	if(access(FIFO_SRV, F_OK) == 0){
		printf("SRV ja em execucao!\n");
		exit(2);
	}


	ncaixas = getenv("NCAIXAS");
	nCaixas = atoi(ncaixas);

	mkfifo(FIFO_SRV, 0600);

	fdLe = open(FIFO_SRV, O_RDONLY);
	
	close(fdLe);
	return 0;
}


//b CAIXA CENTRAL

void *threadTempo(void *d){

	DADOS pdata;
	pdata = (DADOS)d;

	sleep(pdata->tempo);

	int* res;
	res = (int*) malloc(sizeof(int));
	*res = 0;
	pthread_exit(res);
}


int main(int argc, char const *argv[]){

	int horas, *resposta;
	DADOS dados;
	pcli lista;
	pthread_t tempo;

	horas = atoi(argv[1]);

	pthread_create(&tempo, NULL, threadTempo, (void*)dados);

	pthread_join(threadTempo, (void*)&resposta);
		
	if(*resposta == 0){

		pcli aux = lista;
		while(aux != NULL){
			kill(aux->pid, SIGUSR1);
			aux = aux->prox;
		}
		unlink(FIFO_SRV);	
	}	

	free(resposta);


	return 0;
}

//c CAIXA CENTRAL

int main(int argc, char const *argv[]){

	fd_set fds;
	char comando[10];
	pcli lista;
	TDATA data;

	FD_ZERO(&fds);	
	FD_SET(0,&fds); 
	FD_SET(fdLe, &fds);
		
	res = select(fd+1,&fds,NULL,NULL, NULL);

	int fdLe = open(FIFO_SRV, O_RDONLY);

	printf("COMANDO");

	if(res == 0){
		printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
	}
	else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo teclado
		scanf("%s", comando);

		if(strcmp(comando, "anular") == 0){
			pcli aux = lista;
			int codigoAnular, i=0;

			printf("Codigo do produto? \n");
			scanf("%d", &codigoAnular);

			while(aux!=NULL && aux->data->codigo[i] != codigoAnular && i<5){
				aux = aux->prox;
			}

			aux->data->produtos[1][i] = NULL;
		}		


	}
	else if(res > 0 && FD_ISSET(fdLe,&fds)){ //se vier pelo fifo do srv
		int n = read(fdLe, &data, sizeof(TDATA));

		if (strcmp(data.pedido, "registar") == 0){
			
			for (int i = 0; i < 5; i++){
				
				if(data.produtos[1][i] != NULL){
					strcpy(data.produtos[1][i], data.produto_aRegistar);
					data.precos[i] = precoProduto(data.codigos[i]);
				}
			}
		}
	}
	
	close(fdLe);
	return 0;
}

//d CAIXA DO CLIENTE

int fechou = 0;

void recebe(int s, siginfo_t *info, void *context){
	
	printf("Caixa central vai fechar!\n");
	printf("A SAIR...\n");

	fechou = 1;
}

int main(int argc, char const *argv[]){

	int fdEsc;
	char fifo[40];
	TDATA data;

	fdEsc = open (FIFO_SRV, O_WRONLY);  //abrir fifo do serv para escrever
	if(fdEsc < 0){
		printf("[ERRO]Servidor nao abriu!");
		exit(2);
	}

	struct sigaction act;

	act.sa_sigaction = fechou;
	act.sa_flags = SA_SIGINFO;	//ao usar siginfo temos de usar em cima sigaction
	sigaction(SIGUSR1, &act, NULL);

	data.pidCli = getpid();

	sprintf(fifo, FIFO_CLI, data.pidCli);

	mkfifo(fifo,0600); //fifo do cliente criado

	if (fechou == 1){
		unlink(fifo);
		close(fdEsc);
		exit(0);
	}
	
}


//e 

//no cliente

int main(int argc, char const *argv[]){

	int fdEsc;
	char fifo[40], comando[10];
	TDATA data;

	fdEsc = open (FIFO_SRV, O_WRONLY);  //abrir fifo do serv para escrever
	if(fdEsc < 0){
		printf("[ERRO]Servidor nao abriu!");
		exit(2);
	}

	struct sigaction act;

	act.sa_sigaction = fechou;
	act.sa_flags = SA_SIGINFO;	//ao usar siginfo temos de usar em cima sigaction
	sigaction(SIGUSR1, &act, NULL);

	data.pidCli = getpid();

	sprintf(fifo, FIFO_CLI, data.pidCli);

	mkfifo(fifo,0600); //fifo do cliente criado

	printf("Comando? ");
	scanf("%s", data.pedido);

	if (strcmp(comando, "inicio") == 0){
		int n = write(fdEsc, &data, sizeof(TDATA));
	}

	if (fechou == 1){
		unlink(fifo);
		close(fdEsc);
		exit(0);
	}
	
}


//na caixa central

void *incio(void *d){

	TDATA pdata;
	pdata = (TDATA*) d;

	int fdLe = open(FIFO_SRV, O_RDONLY);	//abrir de novo para ler os pedidos do cliente

	int n = read(fdLe, &pdata,sizeof(TDATA));

	if (strcmp(pdata->pedido, "registar") == 0){
			
		for (int i = 0; i < 5; i++){
				
			if(pdata->produtos[1][i] != NULL){
				strcpy(pdata->produtos[1][i], pdata->produto_aRegistar);
				pdata->precos[i] = precoProduto(pdata->codigos[i]);
			}
		}
	}	

	int* res;
	res = (int*) malloc(sizeof(int));
	*res = 0;
}


int main(int argc, char const *argv[]){

	fd_set fds;
	char comando[10];
	pcli lista;
	TDATA data;
	pthread_t inicar;

	FD_ZERO(&fds);	
	FD_SET(0,&fds); 
	FD_SET(fdLe, &fds);
		
	res = select(fd+1,&fds,NULL,NULL, NULL);

	int fdLe = open(FIFO_SRV, O_RDONLY);

	printf("COMANDO");

	if(res == 0){
		printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
	}
	else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo teclado
		scanf("%s", comando);

		if(strcmp(comando, "anular") == 0){
			pcli aux = lista;
			int codigoAnular, i=0;

			printf("Codigo do produto? \n");
			scanf("%d", &codigoAnular);

			while(aux!=NULL && aux->data->codigo[i] != codigoAnular && i<5){
				aux = aux->prox;
			}

			aux->data->produtos[1][i] = NULL;
		}		


	}
	else if(res > 0 && FD_ISSET(fdLe,&fds)){ //se vier pelo fifo do srv
		int n = read(fdLe, &data, sizeof(TDATA));

		if(strcmp(data.pedido, "inicio") == 0){

			pthread_create(&inicar, NULL, incio, (void *)&data)
		}
	}
	

	int *resposta;
	pthread_join(inicio, (void*)&resposta);
	free(respsota);

	close(fdLe);
	unlink(FIFO_SRV);

	return 0;
}