--> Parte 1

1.	
ls -l /etc | wc -l

2.	
cat /etc/passwd | tr -s " " |   cut -d":" -f5 | grep "Santos" | wc -l


-->Parte 2

3.

//srv recebe mensagem de um cliente e envia ao resto dos clientes
//mensagem -> PID, named_pipe, texto 


typedef struct {
    char mensagem[50];
    int pid;
    char nomePipe[10];
}TDATA;


typedef struct cli cliente, *pcli;
struct cli{
	char nomeCli[20];
	char nomePipe[10];
	int pid;
	int enviouMsg;
	pcli prox;
};




#define FIFO_SRV "fifo_srv"
#define FIFO_CLI "fifo_cli%d"


//SERVIDOR

int verifica_cli(pcli lista, int pid){

	pcli atual = lista;

    while(atual !=NULL && atual->pid != pid){	//percorrer até encontrar ou não o pid
        atual = atual->prox; 
    } 

    if(atual == NULL)	//cliente ainda nao exisite
    	return 0;
    else				//ja existe
    	return 1;
}


pcli add_cliente (pcli lista, TDATA data){

	pcli novo, aux;

	novo = malloc(sizeof());

	if (novo == NULL){
		return lista;
	}

	//preencher
   	novo->pid = data.pid;
   	strcpy(novo->nomePipe, data.nomePipe);
   	novo->enviouMsg = 1;	//colocar flag a 1 pq este cliente enviou mensagem

   	novo->prox = NULL;

   	if(lista == NULL){
   		lista = novo;
   	}
   	else{
   		aux = lista;

   		while(aux->prox != NULL){
   			aux = aux->prox;
   		}
   		aux->prox = novo;
   	}

   	return lista;
}

pcli reset_flag(pcli lista){

	while(lista != NULL){
		lista->enviouMsg = 0;
		lista = lista-> prox;
	}

	return lista;
}

pcli mete_flag(pcli lista, int pid){

	pcli atual = lista;

    while(atual !=NULL && atual->pid != pid){	//percorrer até encontrar o pid e meter a flag
        atual = atual->prox; 
    } 

    atual->enviouMsg = 1;

    lista = atual;
    
    return lista; 
}

int verifica_flag(pcli lista){

	while(lista != NULL){
		if(lista->enviouMsg == 0){
			return 1;	//1 que dizer que ha clis que ainda nao enviaram msg
		}
		lista = lista->prox;
	}
	
	return 0;
}


pcli elimina_clientes(pcli lista){

	pcli aux = lista;

	while(aux != NULL){

		if(aux->enviouMsg == 0)
			free(aux);

		aux = aux->prox;
	}

	lista = aux;

	return lista;
}

void *threadTempo(void *dados){

	pcli pdata;
    pdata = (pcli *)dados; 
    int* res;
	
	res = (int*) malloc(sizeof(int));

    sleep(30);		//espera 30s

    if(verifica_flag(pdata) == 1)	//verificar se alguem tem a flag ainda 0
    	*res = 1;
    else
    	*res = 0;

	pthread_exit(res);
}


int main(int argc, char const *argv[]){

	int fdLe, n, fdEscr;
	TDATA data;
	pcli lista, aux;
	pthread_t threadTempo;

		//verifica se ja existe algum srv a correr
	if(access(FIFO_SRV, F_OK) == 0){
		printf("Servidor ja a executar\n");
		exit(2);
	}

		//criar named pipe do srv
	mkfifo(FIFO_SRV, 0600);

	do{
		pthread_create(&threadTempo, NULL, threadTempo, (void*)lista);	//thread para contar 30s


		fdLe = open(FIFO_SRV, O_RDONLY);	//ler o que veio para o fifo do srv

		n = read(fdLe, &data, sizeof(TDATA));	//neste data guardamos a mensagem que acabámos de receber

		if(verifica_cli(lista, data.pid) == 0){	//se ainda nao exisitr, adicionar
			lista = add_cliente(lista, data);	
		}
		else
			lista = mete_flag(lista, data.pid);	//metemos apenas a flag no cliente em questão


		aux = lista;
		while(aux != NULL){
				//mandar msg a todos menos ao que enviou agr msg ao srv
			if(aux->pid != data.pid){	
				
				fdEscr = open(aux->nomePipe, O_WRONLY); //abrir o pipe de cada cliente
				n = write(fdEscr, &data, sizeof(TDATA)); //depois o cli apenas le o que esta no campo .mensagem desta estrutura
				close(fdEscr);
			}
			aux = aux-> prox;
		}
		
		int *res;
		pthread_join(threadTempo, (void*)&res);

		if(*res == 1){	//eliminar clientes pois a thread disse que havia com flag a 0
			lista = elimina_clientes(lista):
		}

		free(res);
	
	}while(1);

	close(fdLe);
	exit(0);
}




//*******CLIENTE********

void mostra(int s){
	continua = 0;
}


int main(int argc, char const *argv[]){

	int n, fdEscr, fdLe, res;
	char fifo[40];
	TDATA data;
	struct sigaction act;
	fd_set fds;

	act.sa_handler = mostra;
	act.sa_flags = 0

	sigaction(SIGUSR1, &act, NULL);

		//preencher estrutura com as informações
	data.pid = getpid();
	sprintf(fifo,FIFO_CLI,data.pid);	//concatenar tudo no nome do fifo

	fdLe = open(fifo, O_RDONLY);	//abrir para ler o que o srv me manda

	FD_ZERO(&fds);	
	FD_SET(0,&fds); 
	FD_SET(fdLe, &fds);
		
	res = select(fd+1,&fds,NULL,NULL, NULL); 


	if(res == 0){
		printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
	}
	else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo teclado, receber e enviar ao srv

		printf("Texto da mensagem: \n"); fflush(stdout);
		scanf("%[^\n]",data.mensagem);

		strcpy(data.nomePipe, fifo);

		mkfifo(data.nomePipe, 0600);	//criar fifo do cliente


		//ESCREVER MENSAGEM AO SRV
			 //abrir fifo do srv para lhe escreever a msg
		fdEscr = open(FIFO_SRV, O_WRONLY);
			//escrever mensagem ao srv
		n = write(fdEscr, &data, sizeof(DATA));	
		close(fdEscr);
	}
	else if(res > 0 && FD_ISSET(fdLe,&fds)){ //se vier pelo fifo do cliente, foi o que o srv enviou

		n = read(fdLe, &data, sizeof(DATA));
		printf("Mensagem recebida pelo SRV: %s\n", data.mensagem);
	}

	close(fdLe);
	close(fdEscr);

	exit(0);
}
 	
