#include "ex1Utils.h"

typedef struct {
    char letra;
    int continua;
    pthread_mutex_t *ptrinco;
}TDATA;


/*com o trinco, garantimos que cada thread faz a sua coisa sem ser interrompida por outras que também queiram comer alguma coisa
	uma thread imprime os A todos que tem a imprimir
	outra thread imprime os B so depois da thread dos A acabar
isto acontece porque quando a thread dos começa a imprimir a sua letra (zona critica, onde ela nao quer ser interrompida)
fecha o trinco e ninguem a interrompe
quando acabar, desbloqueia o trinco e assim a dos B ja pode fazer a sua cena*/


 // tarefa #1
void *mostra(void *dados){

    int *res;

    TDATA *pdata;
    pdata = (TDATA *)dados; 

    do{
        // mostro o nr de pontos aleatoriamente de 1...5
        for(int i = 0; i < rand()%5; i++){
            printf(".");
            fflush(stdout);
            sleep(1);
        }

        //TRANCAR() - queremos que ninguem interrompa enquanto a thread está a meter as letras
        	//para nao termos letras diferentes de threads diferentes a aparecer no ecrã

        /*ZONA CRITICA - zona onde a thread nao quer ser interrompida*/
        pthread_mutex_lock(pdata->ptrinco);

        // imprime a letra
        for(int i = 0; i < 3; i++){ // queremos garantir que isto nao é interrompido
            printf("%c", pdata->letra);
            fflush(stdout);
            sleep(1);
        }

        //DESTRANCAR - thread já imprimiu o que queria, pode tirar o trinco para outras threads 
        pthread_mutex_unlock(pdata->ptrinco);

    }while(pdata->continua);

    res = (int *) malloc(sizeof(int)); // alocamos dinamicamente porque a res pertence a esta funcao e assim na main temos acesso a ela para saber como a thred saiu
    *res = 123;
    pthread_exit(res); // termina apenas esta thread 

}



int main(void){
    char str[40];
    int *resultado;
    TDATA tinfo[NTHREADS];
    pthread_t tarefa[NTHREADS];
    pthread_mutex_t trinco;

    srand((unsigned int)time(NULL));

    //CRIAR_TRINCO() - todas as threads usam-no
    pthread_mutex_init(&trinco,NULL);

    printf("Sou o processo %d...\n",getpid());
    // guardar o identificador da thread (&tarefa), funcao associada à thread (mostra) 


    // aqui estamos a criar 3 threads
    for(int i = 0; i < NTHREADS; i++){
        
        tinfo[i].letra = 'A' + i; // A , B, C
        tinfos[i].continua = 1;
		tinfo[i].ptrinco = &trinco;	//trinco de cada thread vai ser o mesmo para todas
        
        pthread_create(&tarefa[i],NULL,mostra,(void *)&tinfo[i]);
    }


    //main thread
    do{
        printf("COMANDO:\n");
        scanf("%s", str);
        printf("Recebi o comando '%s'...\n",str);

    }while(strcmp(str,"sair")!= 0);


    for(int i = 0; i < NTHREADS; i++){
        printf("Vou pedir a thread #%d para terminar ... ",i+1); fflush(stdout);
        tinfo[i].continua = 0;
        pthread_join(tarefa[i],(void *)&resultado); // esperar pelo fim da thread que criei
        printf("Terminou [%d]!!!\n", *resultado);

        free(resultado);

    }

    //ELIMINAR_TRINCO - aqui ja nao é preciso, estamos mesmo antes do exit(0)
    pthread_mutex_destroy(&trinco);


    exit(0);
}