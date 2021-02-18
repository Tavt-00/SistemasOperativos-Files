#include "utils.h"

typedef struct {
    char letra;
    int continua;
}TDATA;



/* tarefa #1*/
void *mostra(void *dados){
    
    int *res;   //ponteiro porque vamos alocar memoria

    TDATA *pdata;
    pdata = (TDATA *)dados; 

    do{
        // mostrar numero de pontos aleatoriamente de 1...5
        for(int i = 0; i < rand()%5; i++){
            printf(".");
            fflush(stdout);
            sleep(1);
        }

        // mostro a letra
        for(int i = 0; i < 3; i++){
            printf("%c", pdata->letra);
            fflush(stdout);
            sleep(1);
        }
    }while(pdata->continua);

    res = (int *) malloc(sizeof(int)); // alocamos dinamicamente porque a res pertence a esta funcao e assim na main temos acesso a ela para saber como a thred saiu
    *res = 123;
    pthread_exit(res); // termina apenas esta thread 

    
}

int main(void){

    char str[40];
    int *resultado;
    TDATA tinfo[NTHREADS];  //aloco espaço para o nº de threads necessário
    pthread_t tarefa[NTHREADS]; //criar as tarefas necessárias

    srand((unsigned int)time(NULL));


    printf("Sou o processo %d...\n",getpid());
    

    // aqui estamos a criar 3 threads adicionais
    for(int i = 0; i < NTHREADS; i++){
        
        tinfo[i].letra = 'A' + i; // A , B, C
        tinfo[i].continua = 1;
        
        pthread_create(&tarefa[i],NULL,mostra,(void *)&tinfo[i]);
            //criamos uma thread que tem uma funcao associada a ele que faz o que quisermos nessa funcao
            //neste caso estamos a fazer os pritns de a
            //so lhe podemos passar informação e receber pela estrutura (tinfo neste caso)
    }  


    do{
        /*tarefa #0 --> a que ja era do programa quando criado*/

        printf("COMANDO:\n");
        scanf("%s", str);
        printf("Recebi o comando '%s'...\n",str);

    }while(strcmp(str,"sair")!= 0);

    for(int i = 0; i < NTHREADS; i++){
        
        printf("Vou pedir a thread #%d para terminar ... ",i+1); fflush(stdout);
        tinfo[i].continua = 0;  //dizer à thread um (que temos la para cima) para terminar

        pthread_join(tarefa[i],(void *)&resultado); // esperar pelo fim da thread que criei
            //naquele pareamtro resultado vem o que doi passado em pthread_exit na funcao mostra
        
        printf("Terminou [%d]!!!\n", *resultado);

        free(resultado);    //so fazemos free no para nao perder o conteudo
    }


    exit(0);    //termina o processo todo
}
