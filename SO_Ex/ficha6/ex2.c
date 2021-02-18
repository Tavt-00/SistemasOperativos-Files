// uma thread que trata de cada do fifo da esquerda e outra da direita

#include "ex2Utils.h"


void acorda(int s){
	//não faz nada porque apenas queremos quebrar o read da funcao mostra
	//queremos parar de estar ali naquela funcao quando o user quiser sair
}

void *mostra(void *dados){

    int *res, fd, n;
    struct sigaction act;
    PEDIDO p;

    TDATA *pdata;
    pdata = (TDATA *)dados; 

    res = (int *) malloc(sizeof(int));  //nº de pedidos que chegaram do cliente
    *res = 0;

    act.sa_handler = acorda; //--> aqui fazemos apenas handler porque a acorda nao faz nada, nem precisamos de mais informação
    act.sa_flags = 0;
    
    	//tratar do sinal - sigaction() -> sinal quando queremos que user saia logo
   	sigaction(SIGUSR1, &act, NULL);

        /*CRIAR FIFO - mkfifo - para ler a info que vem do cliente*/
   	mkfifo(pdata->nomeFifo, 0600);
    
    	/*abrir fifo (O_RDWR) - open ()*/
   	fd = open(pdata->nomeFifo, O_RDWR);



    do{	//ir lendo enquanto o user nao mete sair

          	//RECEBER INFORMAÇÃO QUE VEM DO CLIENTE - read() - bloqueia
   		n = read(fd, &p, sizeof(p));

   		if(n == sizeof(PEDIDO)){

   				//fechar trinco na zona critica
   			pthread_mutex_lock(pdata->ptrinco);
        
        		//ENVIAR INFORMAÇÃO PARA O MONITOR
   			printf("Fifo: %s, nome user: %s, numero: %d\n", pdata->nomeFifo, p.nome, p.num);

        		//DESTRANCAR - thread já imprimiu o que queria, pode tirar o trinco para outras threads 
        	pthread_mutex_unlock(pdata->ptrinco);

        	(*res)++;
   		}
        
    
    }while(pdata->continua);

   		//AVISAR O CLIENTE PARA SAIR - kill()
 	kill(p.pid, SIGUSR1);

 		//FECHAR FIFO - close
 	close(fd);	

 		//eliminar fifo
    unlink(pdata->nomeFifo);

    pthread_exit(res); // termina apenas esta thread 
}



int main(void){
    
    char str[40];
    int *resultado;
    TDATA tinfo[2];
    pthread_t tarefa[2];	// -> tem o id da thread
    pthread_mutex_t trinco;


    //CRIAR_TRINCO() - todas as threads usam-no
    pthread_mutex_init(&trinco,NULL);

    printf("Sou o processo %d...\n",getpid());


    /*ENVIAR UMA STRING A THREAD - "ESQ" ou  "DIR"*/
    strcpy(tinfo[0].nomeFifo,"ESQ");
    tinfo[0].continua = 1;
    tinfo[0].ptrinco = &trinco;
    pthread_create(&tarefa[0],NULL,mostra,(void *)&tinfo[0]);

    /*ENVIAR UMA STRING A THREAD - "ESQ" ou "DIR"*/
    strcpy(tinfo[1].nomeFifo,"DIR");
    tinfo[1].continua = 1;
    tinfo[1].ptrinco = &trinco;
    pthread_create(&tarefa[1],NULL,mostra,(void *)&tinfo[1]);



    //main thread, thread 0
    do{
        printf("COMANDO:\n");
        scanf("%s", str);
        printf("Recebi o comando '%s'...\n",str);

    }while(strcmp(str,"sair")!= 0);


   
    //termiar as duas threads que criei
    for(int i = 0; i < 2; i++){
        printf("Vou pedir a thread #%d para terminar ... ",i+1); fflush(stdout);
        tinfo[i].continua = 0;

        	//enviar SIGUSR1 à thread para desbloquear o read - pthread_kill() e acabar com as threads
        pthread_kill(tarefa[i], SIGUSR1);

        pthread_join(tarefa[i],(void *)&resultado); // esperar pelo fim da thread que criei
        printf("Terminou [%d]!!!\n", *resultado);

        free(resultado);

    }

    //ELIMINAR_TRINCO - aqui ja nao é preciso, estamos mesmo antes do exit(0)
    pthread_mutex_destroy(&trinco);

    exit(0);
}