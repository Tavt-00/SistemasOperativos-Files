
//PARTE 2 - modidifcado nas threads para testar na m.virtual

1. ls -l /tmp | tr -s " " | tail -n +2 | grep ".txt" | grep "antonio" |  cut -d" " -f5,9 | sort -n -k1 | cut -d" " -f2 > resultado.txt


#include <fcntl.h>
#include <limits.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <ctype.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <string.h>
#include <dirent.h>
#include <errno.h>
#include <sys/select.h>
#include <sys/time.h>
#include <string.h>
#include <pthread.h>
#include <signal.h>
 
#define NPALAVRAS 3
 
typedef struct{
 
    int continua;
    int total;
    char palavra[30];
    char fich[30];
    pthread_mutex_t *ptrinco;
}TDATA;
 
 
int conta(int a, int b){
    return a+b;
}include
 
 
void *find(void *dados){
 
    TDATA *pdata = (TDATA*) dados;
    
    for(int i = 0; i < rand()%5; i++){
        printf(".");
        fflush(stdout);
        sleep(1);
    }
 
    pthread_mutex_lock(pdata->ptrinco);
 
    pdata->total = pdata->total + conta(1, 2);
 
    pthread_mutex_unlock(pdata->ptrinco);
 
    int *res;
    res= (int*)malloc(sizeof(int));
    *res = pdata->total;
    
    pthread_exit(res);
}
 
int main(){
 
    int contador = 0;
    char palavras[NPALAVRAS][10]={"Lisboa", "Porto","Coimbra"};
    pthread_t tre[NPALAVRAS];
    TDATA tinfo[NPALAVRAS];
    int *total;
    pthread_mutex_t trinco;
 
    pthread_mutex_init(&trinco,NULL);
 
    for(int i=0; i< NPALAVRAS; i++)
    {
         tinfo[i].continua = 0;
         tinfo[i].total = 0;
         tinfo[i].ptrinco = &trinco;    
         //strcpy(tinfo[i].palavra,palavras[i]);
         pthread_create(&tre[i],NULL, find,(void*)&tinfo);
    }
 
    for(int i=0; i< NPALAVRAS; i++){
        pthread_join(tre[i],(void *) &total);
        //contador = contador + tinfo[i].total;
        //printf("contador da thread[%d] = %d\n", i, tinfo[i].total);
                //contador = contador + (*total);
 
        //contador = contador + (*total);
    }
 
    printf("Foram registadas %d palavras.", *total);
    pthread_mutex_destroy(&trinco);
    free(total);
    
 exit(0);
 
}




//PARTE 3

typedef struct
{
	char fifo2[40], fifo3[40];
	int numP, tamP;
	int valor;
}TDATA;

//se numP = 3 e tamP = 2

// ->vai enviar pelo FILES_SERVER o FICHEIRO em 3 partes e cada parte tem 2 bytes
	// ->enviando no FIFO2 3 mensagens de 2 bytes cada
		//->cliente le estas mensagens no FIFO2
			//->cada uma das 3 partes, escreve no FIFO3 o VALOR
			//->escreve num ficheiro as 3 partes 



//cliente
int main(int argc, char const *argv[]){

	int fdLe, fdEsc, n;
	char mensagem[1020];
	char nomeFich = "Ficheiro-versao.txt"
	TDATA data;
	FILE *f;

		//fazer pedido ao srv
	fdEsc = open(files_server, O_WRONLY);
	n = write(fdEsc, &nomeFich, sizeof(nomeFich))
	close(fdEsc);

	f = fopen(nomeFich, "w");

		//ler o nome dos dois pipes, numP e tamP
	fdLe = open(files_server, O_RDONLY);
	n = read(fdLe, &data, sizeof(TDATA));
	close(fdLe);

		//ler no 2º pipe
	fdLe = open(data.fifo2, O_RDONLY);
		//e escrever no 3º
	fdEsc = open(data.fifo3, O_WRONLY);

	for (int i = 0; i < data.numP; i++){
		n = read(fdLe, &mensagem, sizeof(data.tamP));

			//ver se houve algo erro na leitura
		if(n == data.tamP){
			data.valor = 1;	//ainda estamos no for(ainda não leu tudo) e a leitura correu bem, queremos a proxima parte	
			fprintf(f, "%s", smensagem);	//está fixe, escreve no ficheiro
		}
		else
			data.valor = 0;
		
		n = write(fdEsc, &data, sizeof(TDATA));	//dizer ao srv se é 0 ou 1
	}

	close(fdLe);
	close(fdEsc);
	return 0;
}