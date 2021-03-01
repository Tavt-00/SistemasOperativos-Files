1.
a. (ls -l /tmp | tr -s " " | tail -n +2 | cut -d" " -f1,5,9 | grep "^[-d]r--") | sort -n -k2 | grep " 0 " | cut -d" " -f3 | sort >> top.txt

b. cat /etc/passwd | grep "^[aeiu]" | cut -d":" -f3 | sort -n -r | wc -l

2.
a.

	i. 1 named pipe para o servidor, para que cada cliente mande pedidos por ele e o próprio srv o use para ler esses pedidos.
		1 named pipe para cada cliente, para o srv escrever neles o resultado do pedido de cada cliente

	ii.	estrutura para passar informações entre cliente e srv, com: nº do quarto atribuído , nome do cliente, gravida, idade, nomeFifo

	iii.  uso do select no cliente para este receber informações pelo teclado ou pelos pipes
		no servidor uso de sinais para avisos do paquete para com o rececionista

b.

#define FIFO_SERV "fifo_srv"

//SERVIDOR
typedef struct
{
	char nome, *condi;
	int idade;
	int numRecQuarto;
	int numRecfifo;
	pthread_mutex_t *ptrinco;
}TDATA;



void *mostra(void * dados){

	TDATA *data = (TADATA *)dados;
	int *res;
	int fdLe, n;

	fdLe = open(FIFO_SERV, O_RDONLY);

	n = read(fd,&data,sizeof(TDATA));

	res = (int*)(malloc(sizeof(int)));

	pthread_mutex_lock(data->ptrinco);

	if((strcmp(data->condi,"gravida") == 0) || data->idade <=3 )
		*res=1;
	else
		*res=0;

	pthread_mutex_unlock(data->ptrinco);
	
	pthread_exit(res);
}



int main(){

	int numRec, n;
	int total, fdEsc;

	numRec = atoi(getenv("RECEPCAO"));

	TDATA data[numRec];
	int prioridades[numRec];
	pthread_t recp[numRec];
	pthread_mutex_t trinco[numRec];

	pthread_mutex_init(&trinco[],NULL);

	mkfifo(FIFO_SERV, 0600);

	for(int i=0; i < numRec; i++){
		data[i]->ptrinco = &trinco;
		pthread_create(&recp[i], NULL, mostra, (void *)&data);
	}

	for(int i=0; i < numRec; i++){	//guardar todos os resultados
		pthread_join(mostra, (void*)(&total));
		prioridades[i]=total;
	}

	for(int i=0; i< numRec; i++){

		if(prioridades[i]==1){

			fdEsc=open(data[i].numRecfifo, O_WRONLY);
			
			data[i].quarto = srand()%200;
			
			n = write(fdEsc, &data, sizeof(TDATA));
			
			prioridades[i]=-1;
		}
	}

	for(int i=0; i< numRec; i++){

		if(prioridades[i]==0){

			fdEsc=open(c[i].numRecfifo, O_WRONLY);
			
			data[i].quarto = srand()%200;
			
			n = write(fdEsc, &data, sizeof(TDATA));
			
			prioridades[i]=-1;
		}
	}


	pthread_mutex_destroy(&trinco);
		
	free(total);
	exit(0);
}

typedef struct 
{	
	int ola;
	int xau;
}TDATA;