// ./cracker palavra_ecriptada tempo_limite

#define FIFO_SRV "tubo"
#define FIFO_TES "testa"

int encontrou = 0, timeout = 0;
PALAVRA palavra;

typedef struct {
    char letra;
    int continua;
    char fifo[20];
    char palaEcrip[10];

}TDATA;


typedef struct {
	char palaEncotrou[10];
}PALAVRA;


void timeout(){

	printf("TIMEOUT!!\n");
	unlink(FIFO_SRV);
}


void *procura(void *dados){

	int saida, *res;
	TDATA data;

	TDATA *pdata;
    pdata = (TDATA *)dados; 

    res = fork();

    if (res == 0){
    	
    	execl("testa", "testa", NULL);
    	exit(2);
    }

    wait(&saida)

    res = (int *) malloc(sizeof(int)); 
    *res = saida;
    pthread_exit(res); 


}


int cracker(int argc, char const *argv[]){

	int tempo, fd, fdr, n, res;
	pthread_t tarefa[27];
	TDATA tinfo[27];

	if (argc != 3)
		exit(2);

	fflush(stdout);

	tempo = atoi(argv[3]);

	signal(SIGALRM, timeout);

	if(access(FIFO_SRV, F_OK) != 0){ 	//criar pipe
		mkfifo(FIFO_SRV,0600);
	}


	fd = open(FIFO_SRV, O_RDONLY);	//de onde le a informação dos filhos

	for (int i = 0; i < 27; i++){
		
		tinfo[i].letra = 'a' + i;

		fdr = open(FIFO_TES, O_WRONLY);
		n = write(fdr, &tinfo[i], sizeof(TDATA));
		
		pthread_create(&tarefa[i], NULL, testa, (void *) &tinfo[i]);
	}

	alarm(tempo);


	//só vem para aqui se encontrar antes do timeout
	if (encontrou){		
			
		n = read(fd, &palavra, sizeof(PALAVRA));

		printf("Palavra desicriptada:  %s\n", palavra.palaEncotrou);
		
		alarm(0);

		close(fd);
		unlink(FIFO_SRV);
		free(res);
	}
	

	return 0;
}







//*********PROGRAMA TESTA*********
  	
void termina(int s){
	exit(0);
}


int main(int argc, char const *argv[]){
	
  	int fd, fdr, n;
  	char teste[10];
	TDATA data;
	PALAVRA palavra;

	struct sigaction act;

	act.sa_handler = termina; //--> aqui fazemos apenas handler porque a acorda nao faz nada, nem precisamos de mais informação
    act.sa_flags = 0;

    sigaction(SIGUSR1, &act, NULL);
    

  	mkfifo(FIFO_TES, 0600);

    fd = open(FIFO_TES, O_RDONLY):	//para ler a letra que vem do craker
    fdr = open(FIFO_SRV, O_WRONLY);	//para lhe escrever a palavra encontrada

    n = read(fd, &data, sizeof(TDATA));

    strcpy(teste, descobre(&data.palaEcrip));

    if(strcmp(teste, NULL) == 0){
    	exit(0);
    }
    else{
    	strcpy(palavra.palaEncotrou, teste);
    	n = write(fdr, &palavra, sizeof(PALAVRA));

    	unlink(FIFO_TES);
    	exit(0);
    }

}