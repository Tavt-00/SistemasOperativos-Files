/*bingo -> dá cartões aos jogadores 
		-> extrai bolas

jogadores -> fazem pedidos (CARTAO ou DESISTO) ao bingo por named pipes a qualquer instante e só pode ter um cartao

-> se bingo recebe CARTAO, deve enviar ao jogador (guardar pid?) um conjunto de 9 numeros aleatorios de 0 a 99 - representa um cartao
-> se bingo recebe DESISTO, deve eliminar o cartao e o proprio jogador da lista de jogadores

-> COMEÇA O BINGO - gerar um numero aletorio a cada N (passado na linha de comandos) segundos 
				  - comunicar esse numero e todos os jogadores
				  - 

	termina quando um jogador preenche todos os numeros

FIFO_SRV onde srv recebe os pedidos dos jogadores

*/

//******BINGO***********


#define FIFO_SRV "fifo_srv"
#define FIFO_CLI "fifo_cli%d"


typedef struct {
    char pedido[10];
    int cartao[9];
    int pid;
}TDATA;


typedef struct{
	int tempo;
	int numeroGerado;
}DADOS;

//apenas para gestao do arbitro
typedef struct cli cliente, *pcli;
struct cli{
	int cartao[9];
	char nomePipe[10];
	int pid;
	pcli prox;
};



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


pcli eliminar_cliente (pcli lista, int pid){
	
	pcli atual = lista, ant = NULL;

    while(atual!=NULL atual->pid != pid){	//percorrer até encontrar ou não o nome
        ant = atual; 
        atual = atual->prox; 
    } 

    if(atual == NULL){	// cliente nao existe
        printf("O jogador para remover nao existe!");
        return lista; 
    }   
    else if(atual == lista)      // Primeiro da lista 
        lista = lista->prox; 

    else                            // Outro 
        ant->prox = atual->prox; 

   	free(atual);     

   	return lista;  
}

void *threadTempori(void *d){

	DADOS dados;
	dados = (DADOS) d;

	sleep(dados->tempo);

	int* res;
	res = (int*) malloc(sizeof(int));
	*res_timer = gera_nAleatorio();
	pthread_exit(res);
}


int main(int argc, char const *argv[]){

	int fdLe, fdEsc, n, i;
	int aletorio;
	TDATA data;
	pcli lista;
	DADOS dados;
	pthread_t threadTempo;

	dados.tempo = atoi(argv[1]);

	if(access(FIFO_SRV, F_OK) == 0){
		printf("O bingo ja esta em execucao!\n");
		exit(2);
	}

		//criar named pipe do srv
	mkfifo(FIFO_SRV, 0600);

	fdLe = open(FIFO_SRV, O_RDONLY);

	n = read(fdLe, &data, sizeof(data));
	do{
		if (strcmp(data.pedido, "cartao") == 0){
				
			pcli aux = lista; 

				//ver se já existe o jogador
			while(aux != NULL || aux->pid != data.pid){
				aux = aux->prox;
			}

			if (aux == NULL){	//se está a NULL é pq ainda nao existe
					
					//gerar o cartao do jogador
				for (i = 0; i < 9; i++){	
					aletorio = random(99);
				 	data.cartao[i] = aletorio;
				}

				lista = add_cliente(lista, data);
				strcpy(data.pedido, "entrou");
				fdEscr = open(aux->nomePipe, O_WRONLY); 	
				n = write(fdEscr, &data, sizeof(TDATA)); 
			}
			else{
				strcpy(data.pedido, "jaExiste");
			}
		}
		
		
		else if (strcmp(data.pedido, "desisto") == 0){
			lsita = elimina_cliente(lista, data.pid);
			strcpy(data.pedido, "desistiu");
			fdEscr = open(aux->nomePipe, O_WRONLY); 	
			n = write(fdEscr, &data, sizeof(TDATA)); 
		}
		else{
			printf("SRV so aceita 'desisto' ou 'cartao'\n");
			strcpy(data.pedido, "erroCmd");
		}
	
			//encontrar de novo o jogador para lhe enviar pelo pipe dele
		while(aux != NULL && aux->pid != data.pid){
			aux = aux->prox;
		}
			
			//enviar ao jogador o resultado do pedido dele
		fdEscr = open(aux->nomePipe, O_WRONLY); 	
		n = write(fdEscr, &data, sizeof(TDATA)); 
		close(fdEscr);

		int *resposta;
		pthread_join(threadTempo, (void*)&resposta);

		dados.numeroGerado = resposta;

			//envia numero selecionado a todos os jogadores
		while(aux != NULL){
			fdEscr = open(aux->nomePipe, O_WRONLY); 	
			n = write(fdEscr, &dados, sizeof(DADOS)); 
			close(fdEscr);
			
			kill(aux->pid, SIGUSR1);
			aux = aux->prox;
		}


	}while(1);
	
	free(resposta);
	close(fdLe);

	exit(0);
}



//****JOGADOR********


int recebeu = 0;

void recebe(int s, siginfo_t *info, void *context){
	recebeu = 1;
}

int main(int argc, char const *argv[]){

	int pid,n;
	char fifo[40];
	TDATA data;
	DADOS dados;
	fd_set fds;
	struct sigaction act;

	act.sa_sigaction = recebe;
	act.sa_flags = SA_SIGINFO;	//ao usar siginfo temos de usar em cima sigaction
	sigaction(SIGUSR1, &act, NULL);

	data.pid = getpid();	//para depois enviar tudo ja ao srv

	sprintf(fifo,FIFO_CLI,data.pid)

	mkfifo(fifo,0600); //fifo do cliente criado

	printf("COMANDO: ");

	int fdLe = open(fifo, O_RDONLY);

	FD_ZERO(&fds);	
	FD_SET(0,&fds); 
	FD_SET(fdLe, &fds);
		
	res = select(fd+1,&fds,NULL,NULL, NULL); 

	if(res == 0){
		printf("Nao ha dados recebidos nem pelo fifo nem pelo teclado...\n");
	}
	else if(res > 0 && FD_ISSET(0,&fds)){ //se vier pelo teclado, receber e enviar ao srv

		scanf("%s", data.pedido);

		int fdEscr = open(FIFO_SRV, O_WRONLY);	//abrir o do srv para lhe enviar a msg
		n = write(fdEscr, &data, sizeof(TDATA));
		close(fdEscr);

	}
	else if(res > 0 && FD_ISSET(fdLe,&fds)){ //se vier pelo fifo do cliente, foi o que o srv enviou
		if(recebeu){
			n = read(fdLe, &dados, sizeof(DADOS));
			printf("Numero sorteado: %d\n", dados.numeroGerado);
		}

			//leitura da confirmação, etc
		n = read(fdLe, &data, sizeof(TDATA));
		close(fdLe);
	}
	return 0;
}